//-----------------------main loop------------------------------
/*const int analogInPin = A1; 
int sensorValue = 0; 
unsigned long int avgValue; 
float b;
int buf[10],temp;*/

void loop()
{

//PRUEBA CODIGO CALIBRACION SONDA PH
/* for(int ii=0;ii<10;ii++) 
 { 
  buf[ii]=analogRead(analogInPin);
  delay(20);
 }
 for(int ii=0;ii<9;ii++)
 {
  for(int jj=ii+1;jj<10;jj++)
  {
   if(buf[ii]>buf[jj])
   {
    temp=buf[ii];
    buf[ii]=buf[jj];
    buf[jj]=temp;
   }
  }
 }
 avgValue=0;
 for(int ii=2;ii<8;ii++)
  avgValue+=buf[ii];
  float pHVol=(float)avgValue*5.0/1024/6;
  float phVal = -5.70 * pHVol + 21.34;
  Serial.print("sensor = ");
  Serial.println(phVal);
 
  delay(200);
// FIN CODIGO PRUEBA CALIBRACION PH */


  t = rtc.getTime(); // Atualiza as variáveis que usam o RTC.
  readPh(); // Lee el valor PH de la sonda

  if (executandoNuvem == false)
  {
    LED_levels_output(); // Atualiza a potência de saída dos leds
  }
  else
  {
    checkNuvemRelampago();
  }

  if (NumMins(t.hour, t.min) != lastMinute)
  {
    lastMinute = NumMins(t.hour, t.min);

    if ((haveraNuvem == true) && (executandoNuvem == false))
    {
      for (int i = 1; i <= quantNuvens; i++)
      {
        if (NumMins(t.hour, t.min) == horaNuvem[i])
        {
          executandoNuvem = true;
          inicioNuvem = true;
          millis_nuvem = millis();
        }
      }
    }

    if (NumMins(t.hour, t.min) == 0)
    {
      probabilidadeNuvem(); // Calcula a probabilidade de ocorrer nuvens.
    }
  }

  checktpa(); // Verifica e executa a TPA automática.

  if ((millis() - log_SD_millis) > 60000) // Executa funções a cada 1 minuto.
  {
    selecionar_SPI(SD_CARD); // Seleciona disposito SPI que será utilizado.

    logtempgraf(); // Graba temperatura en tarjeta SD
    logphagraf(); // Graba el PH del acuario en tarjeta SD.
    logphrgraf(); // Graba el PH del reactor de calcio en tarjeta SD.
    logdengraf(); // Graba densidad en tarjeta SD.
    logorpgraf(); // Graba el ORP en tarjeta SD.
    log_SD_millis = millis();
  }

  if ((dosadoras == true) && (bitRead(tpa_status, 1) == false)) // Verifica e executa as dosagens.
  {
    if ((modo_alimentacao == false) || ((millis() < alimentacao_millis) && (modo_alimentacao == true)))
    {
      if ((millis() - dosadoras_millis) > 60000) // Verifica funções a cada 1 minuto.
      {
#ifdef WATCHDOG //Do not change this line
        wdt_disable();
#endif //Do not change this line
        selecionar_SPI(SD_CARD); // Seleciona disposito SPI que será utilizado.
        dosadoras_millis = millis();
        check_dosagem_personalizada(); // Dosagem personalizada
#ifdef WATCHDOG //Do not change this line
        wdt_enable(WDTO_8S);
#endif //Do not change this line
      }
    }
  }

#ifdef ETHERNET_SHIELD //Do not change this line
#ifndef USE_ESP8266 //Do not change this line
  if ((millis() - close_millis) > 1000)
  {
    selecionar_SPI(ETHER_CARD); // Seleciona disposito SPI que será utilizado.
    checkSockStatus();
    close_millis = millis();
  }
#endif //Do not change this line

  if (bitRead(tpa_status, 1) == false)
  {
#ifndef USE_ESP8266 //Do not change this line
    selecionar_SPI(ETHER_CARD); // Seleciona disposito SPI que será utilizado.
    if (!MQTT.connected())
    {
      if ((millis() - millis_mqtt) > 300000) // Tenta reconectar após 5 minutos
      {
        reconnect();
        millis_mqtt = millis();
      }
    }

    MQTT.loop();

#else //Do not change this line
    if (MQTT_connected == false)
    {
      if ((millis() - millis_mqtt) > 300000) // Tenta reconectar após 5 minutos
      {
        sincronizar();
        millis_mqtt = millis();
      }
    }
    ESP8266.Process();
#endif //Do not change this line
  }

  if ((millis() - millis_enviar) > 120000)
  {
    if (bitRead(tpa_status, 1) == false)
    {
      selecionar_SPI(ETHER_CARD); // Seleciona disposito SPI que será utilizado.
      enviar_dados();
      millis_enviar = millis();
    }
  }

  if (outlets_settings == true)
  {
    if ((millis() - outlets_millis) > 300000) // Lê os valores da EEPROM se as configurações não forem salvas em até 5 minutos
    {
      for (byte i = 0; i < 9; i++)
      {
        outlets_changed[i] = true;
      }
      byte k = EEPROM.read(840);

      if (k != 66)
      {
        for (byte i = 0; i < 9; i++)
        {
          outlets[i] = 0;
        }
      }
      else
      {
        ler_outlets_EEPROM();
      }
      outlets_settings = false;
    }
  }
#endif //Do not change this line

#ifdef USE_TFT //Do not change this line
  if (myTouch.dataAvailable())
  {
#ifdef USE_SCREENSAVER //Do not change this line
    if (((millis() - previousMillis_2) > (interval * 1000UL)) && (dispScreen == 0))
    {
      clearScreen();
      mainScreen(true);
    }
    else
    {
      processMyTouch();
    }
    previousMillis_2 = millis();
#else //Do not change this line
    processMyTouch();  // Verifica se o LCD está sendo tocado e faz o processamento.
#endif //Do not change this line
  }
#endif //Do not change this line

#ifdef WATCHDOG //Do not change this line
  wdt_reset();
#endif //Do not change this line

#if defined(STAMPS_EZO) || defined(STAMPS_V4X) //Do not change this line
  if ((millis() - millis_antes) >= 120000) // Executa as funções a cada 2 minutos.
  {

#ifdef USE_STAMP_FOR_CALCIUM_REACTOR //Do not change this line
    check_parametro_ph_reator(); // Verifica o pH do reator de cálcio.
#endif //Do not change this line

#ifdef USE_STAMP_FOR_ORP //Do not change this line
    check_parametro_orp();       // Verifica a ORP
#endif //Do not change this line

#ifdef USE_STAMP_FOR_DENSITY //Do not change this line
    check_parametro_densidade(); // Verifica a densidade
#endif //Do not change this line

#ifdef USE_STAMP_FOR_TANK_PH //Do not change this line
    check_parametro_ph_aquario(); // Verifica o pH do aquário
#endif //Do not change this line

    millis_antes = millis();
  }
#endif //Do not change this line

#ifdef ETHERNET_SHIELD //Do not change this line
  if ((web_dosage == true) && ((millis() - millis_dosagem) > 10000))
  {
#ifdef WATCHDOG //Do not change this line
    wdt_disable();
#endif //Do not change this line
    dosagem_manual();
    web_dosage = false;
#ifdef WATCHDOG //Do not change this line
    wdt_enable(WDTO_8S);
#endif //Do not change this line
  }

  if ((web_calibracao == true) && ((millis() - millis_dosagem) > 10000))
  {
#ifdef WATCHDOG //Do not change this line
    wdt_disable();
#endif //Do not change this line
    calibrar();
    web_calibracao = false;
#ifdef WATCHDOG //Do not change this line
    wdt_enable(WDTO_8S);
#endif //Do not change this line
  }
#endif //Do not change this line

#if defined(RFM12B_LED_CONTROL) || defined(RFM12B_RELAY_CONTROL) //Do not change this line
  if ((millis() - lastPeriod_millis) > 5000)
  {
    if (millis() > 20000)
    {
      selecionar_SPI(RFM); // Seleciona disposito SPI que será utilizado.
#ifdef RFM12B_LED_CONTROL //Do not change this line
      RF_LED();
#endif //Do not change this line
#ifdef RFM12B_RELAY_CONTROL //Do not change this line
      RF_RELAY();
#endif //Do not change this line
    }
    lastPeriod_millis = millis();
  }
#endif //Do not change this line

  if ((dispScreen != 22) && (web_teste == false))
  {
    teste_em_andamento = false;
  }

#ifdef ETHERNET_SHIELD //Do not change this line
  if (web_teste == true)
  {
    if ((millis() - teste_led_millis) > 600000)
    {
      web_teste = false;
      teste_em_andamento = false;
      ler_predefinido_EEPROM();
    }
  }
#endif //Do not change this line

#ifdef USE_TFT //Do not change this line
  if ((dispScreen == 3) && (LEDtestTick == true)) // Imprime valores se o teste de todos os leds em andamento.
  {
    testScreen();
  }
#endif //Do not change this line

  Wavemaker();

#ifdef USE_TFT //Do not change this line
  if (dispScreen == 10) // Desenha os gráficos enquanto o menu estiver aberto.
  {
    if ((modo_alimentacao == false) || (wavemaker_on_off == false))
    {
      Grafico_WaveMaker();
    }
  }
#endif //Do not change this line

  if (modo_alimentacao == true) // Função que desliga alimentador automático.
  {
    if (millis() >= alimentacao_millis)
    {
      if ((millis() - alimentacao_millis) > (duracao_alimentacao * 1000L))
      {
        modo_alimentacao = false;
#ifdef USE_TFT //Do not change this line
        if (dispScreen == 45)
        {
          strcpy_P(buffer, (char*)pgm_read_word_near(&(tabela_textos[17])));
          printButton(buffer, anT[0], anT[1], anT[2], anT[3]); // tabela_textos[17] = "INICIAR"
        }
#endif //Do not change this line
      }
    }
  }

  if (wavemaker_on_off == true) // Função que liga o wavemaker após o período de alimentação.
  {
    if ((millis() - wavemaker_on_off_millis) > (desligar_wavemaker * 60000))
    {
      wavemaker_on_off = false;
    }
  }

  if ((millis() - check_alimentador_millis) > 60000)
  {
    check_alimentador(); // Verifica agendamendo do alimentador.
    check_alimentador_millis = millis();
  }

  alimentador_automatico(); // Liga ou desliga o alimentador automático.

  if (millis() - previousMillis > 5000)    // Verifica as funções a cada 5 segundos.
  {
    checkTempC();  // Verifica as temperaturas.
    #ifdef DEBUG
    Serial.println("Entrando en la función de comprobación de temperatura");
    #endif
    reposicao_agua_doce();  // Verifica se há a necessidade reposição da água doce.
    check_nivel();          // Verifica se há algum problema com os níveis dos aquários.
    check_alarme();         // Verifica os alarmes.
    check_temporizadores(); // Ativa ou desativa os timers.

#if defined(STAMPS_EZO) || defined(STAMPS_V4X) //Do not change this line
#endif //Do not change this line

#ifdef USE_PHReator //Do not change this line
    check_PH_reator(); // Verifica o pH do reator de cálcio.
#endif //Do not change this line

#ifdef USE_ORP //Do not change this line
    check_ORP();       // Verifica a ORP
#endif //Do not change this line

#ifdef USE_Density //Do not change this line
    check_densidade(); // Verifica a densidade
#endif //Do not change this line

#ifdef USE_PHAquarium //Do not change this line
    #ifdef DEBUG
    Serial.println("He entrado en el check del PH");
    #endif
    check_PH_aquario(); // Verifica o pH do aquário
#endif //Do not change this line

#ifdef DISABLE_SKIMMER //Do not change this line
    check_level_skimmer();  // desativa o skimmer para evitar transbordamento.
#endif //Do not change this line

    if (LEDtestTick == false)          // Atualiza se o teste de todos os leds não estiver em andamento.
    {
      min_cnt = NumMins(t.hour, t.min); // Atualiza o intervalo para determinar a potência dos leds.
    }

#ifdef USE_TFT //Do not change this line
    if (dispScreen == 0)
    {
#ifdef USE_SCREENSAVER //Do not change this line
      if ((millis() - previousMillis_2) > (interval * 1000UL))
      {
        clockScreen();  // atualiza o protetor de tela
      }
      else
      {
        mainScreen();  // Atualiza tela inicial.
        firstTime = true;
      }
#else //Do not change this line
      mainScreen();  // Atualiza tela inicial.
#endif //Do not change this line
    }
#endif //Do not change this line

    if (suavizar <= 1)
    {
      suavizar += 0.1;
    }
#ifdef DEBUG //Do not change this line
    /*  Serial.println();
       Serial.print(F("Day of week: ");
       Serial.println(rtc.getDOWStr());

       Serial.print(F("Time: ");
       Serial.println(rtc.getTimeStr(FORMAT_LONG));*/

    Serial.print(F("Free memory: "));
    Serial.println(FreeRam());

      /////////// Código modificado por Miguel Ortega: miorba@gmail.com
      // Sonda PH 
     Serial.print(" Valor sonda PH: ");
     Serial.println(PHA);
     Serial.print(" Valor PH definido: ");
     Serial.println(setPHA);
     Serial.print(" Valor PH desactivado: ");
     Serial.println(offPHA);
     Serial.print(" Valor contador lectura PH: ");
     Serial.println(samples);
     if (PHA > (setPHA + offPHA)){
      Serial.println(" He activado Co2");}
    ///////////////////

    /*
      Serial.print(F("Sensor 1: "));
      Serial.println(myAnalogRead(A0));

      Serial.print(F("Sensor 2: ");
      Serial.println(myAnalogRead(A1));

      Serial.print(F("Sensor 3: "));
      Serial.println(myAnalogRead(A2));

      Serial.print(F("Sensor 4: "));
      Serial.println(myAnalogRead(A3));

      Serial.print(F("Sensor 5: "));
      Serial.println(myAnalogRead(A4));

      Serial.print(F("Sensor 6: "));
      Serial.println(myAnalogRead(A5));

      Serial.print(F("marcadoriniciotpa: "));
      Serial.println(marcadoriniciotpa);

      Serial.print(F("shiftedmillis: "));
      Serial.println(shiftedmillis);

      Serial.print(F("Duration for stage: "));
      Serial.print(tempo/60000);
      Serial.println(F(" minutes"));

      if (digitalRead(bomba1Pin)==HIGH)
      {
      Serial.println(F("Pump 1: ON"));
      }
      else
      {
      Serial.println(F("Pump 1: OFF"));
      }
      if (digitalRead(bomba2Pin)==HIGH)
      {
      Serial.println(F("Pump 2: ON"));
      }
      else
      {
      Serial.println(F("Pump 2: OFF"));
      }
      if (digitalRead(bomba3Pin)==HIGH)
      {
      Serial.println(F("Pump 3: ON"));
      }
      else
      {
      Serial.println(F("Pump 3: OFF"));
      }*/
#endif //Do not change this line
    previousMillis = millis();
  }

} //-------------------end of main loop











