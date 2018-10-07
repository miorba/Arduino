//---------------------------------------------------------------------------------------------
void checkTempC()
{
   outlets[0] = 0;
   outlets[1] = 0;
   #ifdef DEBUG
   Serial.print("El valor Outlets[0] es: ");
   Serial.println(outlets[0]);
   Serial.print("El valor Outlets[1] es: ");
   Serial.println(outlets[1]);
   Serial.println("Entrando en la funcion checkTeempC");
   #endif  
  contador_temp ++;

  sensors.requestTemperatures();                                          // Chamada para todos os sensores.
#ifdef USE_FAHRENHEIT
  temperatura_agua_temp += (sensors.getTempF(sensor_agua));               // Lê temperatura da água
  temperatura_dissipador_temp += (sensors.getTempF(sensor_dissipador));   // Lê temperatura do dissipador
  temperatura_ambiente_temp += (sensors.getTempF(sensor_ambiente));       // Lê temperatura do dissipador
#else
  temperatura_agua_temp += (sensors.getTempC(sensor_agua));               // Lê temperatura da água
  temperatura_dissipador_temp += (sensors.getTempC(sensor_dissipador));   // Lê temperatura do dissipador
  temperatura_ambiente_temp += (sensors.getTempC(sensor_ambiente));       // Lê temperatura do dissipador
#endif

  if (contador_temp == 12)
  {
    tempC = temperatura_agua_temp / 12;
    tempH = temperatura_dissipador_temp / 12;
    tempA = temperatura_ambiente_temp / 12;
    contador_temp = 0;
    temperatura_agua_temp = 0;
    temperatura_dissipador_temp = 0;
    temperatura_ambiente_temp = 0;
  }

//////////////////////////////////////////////////////////////
////// Modificado por Miguel Ortega: miorba@gmail.com ////////
///////////Añadido apagado Enfriador y Calentador al arranque //////////////
  
if (initemperatura==0) // Iniciador de calentador y enfriador
{
    #ifdef DEBUG
    Serial.println("Reiniciando calentador y enfriador");
    #endif
    bitWrite(status_parametros, 1, 0);
    bitWrite(status_parametros, 0, 0);
    myDigitalWrite(aquecedorPin, LOW); // Desactiva el calentador
    myDigitalWrite(chillerPin, LOW); // Desactiva el enfriador
    initemperatura = 1;
}

  if (tempC < (setTempC + offTempC + alarmTempC) && tempC > (setTempC - offTempC - alarmTempC))
  {
    bitWrite(status_parametros, 2, 0); // Desactiva la alarma
  }

  if (alarmTempC > 0)
  {
    if ((tempC >= (setTempC + offTempC + alarmTempC)) || (tempC <= (setTempC - offTempC - alarmTempC)))
    {
      bitWrite(status_parametros, 2, 1); // Activa la alarma
    }
  }

  if (outlets_changed[0] == true)
  {
    outlets_changed[0] = false;
    bitWrite(status_parametros, 1, 0);
    myDigitalWrite(aquecedorPin, LOW);
  }

  if (outlets_changed[1] == true)
  {
    outlets_changed[1] = false;
    bitWrite(status_parametros, 0, 0);
    myDigitalWrite(chillerPin, LOW);
  }

////////////////////////////////////////////////////////////
/////////////// Modificado hasta aquí //////////////////////
////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////
////// Modificado por Miguel Ortega: miorba@gmail.com ////////
///////////Modificado control temperatura apagado (Salta constantemente) //////////////


  if ((outlets[0] == 0) && (outlets[1] == 0))
  {
   #ifdef DEBUG
   Serial.println("Comprobando temperatura para activar calentador o enfriador");
   #endif
  // Cambiado por Miguel Ortega    if ((tempC < (setTempC + offTempC)) && (tempC > (setTempC - offTempC)))          // Apaga calentador y enfriador // Desliga aquecedor e chiller
  if (tempC >= setTempC)          // Apaga calentador y enfriador solo cuando alcanza el valor deseado (Sin el valor de variable aceptada).
    {
      bitWrite(status_parametros, 1, 0);
      myDigitalWrite(aquecedorPin, LOW);
    }
  else if (tempC <= setTempC)
    {
      bitWrite(status_parametros, 0, 0);
      myDigitalWrite(chillerPin, LOW);
    }

/// Cambiado hasta aquí
    if (offTempC > 0)
    {
      if ((tempC > (setTempC + offTempC)))            // Enciende el enfriador
      {
        bitWrite(status_parametros, 0, 1);
        myDigitalWrite(chillerPin, HIGH);
        bitWrite(status_parametros, 1, 0);
        myDigitalWrite(aquecedorPin, LOW);
      }
      else if ((tempC < (setTempC - offTempC)))             // Enciende el calentador
      {
        bitWrite(status_parametros, 1, 1);
        myDigitalWrite(aquecedorPin, HIGH);
        bitWrite(status_parametros, 0, 0);
        myDigitalWrite(chillerPin, LOW);
      }
    }

  #ifdef USE_FAHRENHEIT
    if ((tempC > 122) || (tempC < 50))
  #else
    if ((tempC > 50) || (tempC < 10))
  #endif
    {
      bitWrite(status_parametros, 0, 0);
      bitWrite(status_parametros, 1, 0);
      myDigitalWrite(aquecedorPin, LOW);
      myDigitalWrite(chillerPin, LOW);
    }

    if ((bitRead(status_parametros, 0) == true) &&  (bitRead(status_parametros, 1) == true)) // Evita calentador y enfriador enciendan al mismo tiempo. Evita aquecedor e chiller ligados ao mesmo tempo.
    {
      bitWrite(status_parametros, 0, 0);
      bitWrite(status_parametros, 1, 0);
      myDigitalWrite(aquecedorPin, LOW);
      myDigitalWrite(chillerPin, LOW);
    }
  }

  if (outlets[0] == 1)
  {
    bitWrite(status_parametros, 1, 1);
    myDigitalWrite(aquecedorPin, HIGH); // Enciende el enfriador
  }
  else if (outlets[0] == 2)
  {
    bitWrite(status_parametros, 1, 0);
    myDigitalWrite(aquecedorPin, LOW); // Apaga el enfriador
  }

  if (outlets[1] == 1)
  {
    bitWrite(status_parametros, 0, 1);
    myDigitalWrite(chillerPin, HIGH); // Enciende el calentador
  }
  else if (outlets[1] == 2)
  {
    bitWrite(status_parametros, 0, 0);
    myDigitalWrite(chillerPin, LOW); // Apaga el calentador
  }

  int tempval = int(tempH * 10);
  fanSpeed = map(tempval, (HtempMin * 10), (HtempMax * 10), 0, 255);       // Control de velocidad de los ventiladores del disipador // Controle de velocidade das ventoinhas do dissipador

  if (fanSpeed < 0)
  {
    fanSpeed = 0;
  }
  if (fanSpeed > 255)
  {
    fanSpeed = 255;
  }
  myAnalogWrite(fanPin, fanSpeed);

  if (tempH < HtempMin) // Desactiva los enfriadores si la temperatura está por debajo de la mínimoa definida. // Desativa os coolers se a temperatura estiver abaixo da mínima definida.
  {
    myDigitalWrite(desativarFanPin, LOW);
  }
  else
  {
    myDigitalWrite(desativarFanPin, HIGH);
  }
}

void check_nivel() // Verifica o nível do reef e fish only.
{
  if ((myAnalogRead(sensor5) > 400) || (myAnalogRead(sensor6) > 400) || ((myAnalogRead(sensor2) < 400) && (bitRead(tpa_status, 1) == false)))
  {
    nivel_status1 = true; // Sinaliza nivel baixo em um dos aquários ou sump
  }
  else
  {
    nivel_status1 = false; // Sinaliza nível normal dos aquários e sump
  }

  // Edit this part according to your requirements.

  /*if(myAnalogRead(sensor1) < 400)
    {
    nivel_status1 = true; // Sinaliza nivel baixo sensor 1
    }
    else
    {
    nivel_status1 = false; // Sinaliza nível normal sensor 1
    }
    if(myAnalogRead(sensor2) < 400)
    {
    nivel_status2 = true; // Sinaliza nivel baixo sensor 2
    }
    else
    {
    nivel_status2 = false; // Sinaliza nível normal sensor 2
    }
    if(myAnalogRead(sensor3) < 400)
    {
    nivel_status3 = true; // Sinaliza nivel baixo sensor 3
    }
    else
    {
    nivel_status3 = false; // Sinaliza nível normal sensor 3
    }
    if(myAnalogRead(sensor4) < 400)
    {
    nivel_status4 = true; // Sinaliza nivel baixo sensor 4
    }
    else
    {
    nivel_status4 = false; // Sinaliza nível normal sensor 4
    }
    if(myAnalogRead(sensor5) < 400)
    {
    nivel_status5 = true; // Sinaliza nivel baixo sensor 5
    }
    else
    {
    nivel_status5 = false; // Sinaliza nível normal sensor 5
    }
    if(myAnalogRead(sensor6) < 400)
    {
    nivel_status6 = true; // Sinaliza nivel baixo sensor 6
    }
    else
    {
    nivel_status6 = false; // Sinaliza nível normal sensor 6
    }*/
}

void check_PH_reator()
{
  if (PHR < (setPHR + offPHR + alarmPHR) && PHR > (setPHR - offPHR - alarmPHR))
  {
    bitWrite(status_parametros, 6, 0);
  }

  if (alarmPHR > 0)           // Liga o alarme
  {
    if ((PHR > (setPHR + offPHR + alarmPHR)) || (PHR <= (setPHR - offPHR - alarmPHR)))
    {
      bitWrite(status_parametros, 6, 1);
    }
  }

  if (outlets_changed[3] == true)
  {
    outlets_changed[3] = false;
    bitWrite(status_parametros, 5, 0);
#ifdef USE_PCF8575
    PCF8575.digitalWrite(reatorPin, LOW);  // Desliga co2 do reator de cálcio
#else
    myDigitalWrite(reatorPin, LOW);  // Desliga co2 do reator de cálcio
#endif
  }

  if (outlets[3] == 0)
  {    
    if ((PHR < 4) || (PHR > 10))
    {
      bitWrite(status_parametros, 5, 0);
#ifdef USE_PCF8575
      PCF8575.digitalWrite(reatorPin, LOW);  // Desliga co2 do reator de cálcio
#else
      myDigitalWrite(reatorPin, LOW);  // Desliga co2 do reator de cálcio
#endif
    }
    else
    {
      if (PHR < setPHR)
      {
        bitWrite(status_parametros, 5, 0);
#ifdef USE_PCF8575
        PCF8575.digitalWrite(reatorPin, LOW);  // Desliga co2 do reator de cálcio
#else
        myDigitalWrite(reatorPin, LOW);  // Desliga co2 do reator de cálcio
#endif
      }
      if (offPHR > 0)
      {
        if (PHR > setPHR)
        {
          bitWrite(status_parametros, 5, 1);
#ifdef USE_PCF8575
          PCF8575.digitalWrite(reatorPin, HIGH); // Liga co2 do reator de cálcio
#else
          myDigitalWrite(reatorPin, HIGH); // Liga co2 do reator de cálcio
#endif
        }
      }
    }
  }
  else if (outlets[3] == 1)
  {
    bitWrite(status_parametros, 5, 1);
#ifdef USE_PCF8575
    PCF8575.digitalWrite(reatorPin, HIGH); // Liga co2 do reator de cálcio
#else
    myDigitalWrite(reatorPin, HIGH); // Liga co2 do reator de cálcio
#endif
  }
  else if (outlets[3] == 2)
  {
    bitWrite(status_parametros, 5, 0);
#ifdef USE_PCF8575
    PCF8575.digitalWrite(reatorPin, LOW); // Liga co2 do reator de cálcio
#else
    myDigitalWrite(reatorPin, LOW); // Liga co2 do reator de cálcio
#endif
  }
}

// Cambiado por Miguel Ortega: miorba@gmail.com

void check_PH_aquario()
{
  if (initCo2==0) // Iniciador del Co2
   {
    bitWrite(status_parametros, 5, 0);
    myDigitalWrite(Co2Pin, LOW); // Desactiva el Co2
    initCo2 = 1;
   }
  if (PHA < (setPHA + offPHA + alarmPHA) && PHA > (setPHA - offPHA - alarmPHA))
  {
    bitWrite(status_parametros, 3, 0);
    #ifdef DEBUG
    Serial.println("ha entrado en el condicional de PH y no activa la alarma");
    #endif 
  }
  if (alarmPHA > 0)           // Liga o alarme
  {
    if ((PHA >= (setPHA + offPHA + alarmPHA)) || (PHA <= (setPHA - offPHA - alarmPHA)))
    {
      bitWrite(status_parametros, 3, 1);
    #ifdef DEBUG
    Serial.println("ha entrado en el condicional de PH y ACTIVA la alarma");
    #endif 
    }
  }
  if (outlets[9] == 0)
  {
    if (PHA < (setPHA - offPHA)) // Si PH del acuario menor que el deseado menos la variación permitida.
    {
      bitWrite(status_parametros, 5, 0);
#ifdef USE_PCF8575
      PCF8575.digitalWrite(Co2Pin, LOW);  // Desliga co2 do acuario
#else
      myDigitalWrite(Co2Pin,LOW); //Desliga Co2 do acuario
      #ifdef DEBUG
      Serial.println("ha entrado en el condicional de PH y DESACTIVA el Co2");
      #endif 
#endif
    } else if (PHA > (setPHA + offPHA)) // Si el PH del acuario es mayor que el deseado menos la variación permitida
    {
      bitWrite(status_parametros, 5, 1);
      #ifdef USE_PCF8575
        PCF8575.digitalWrite(Co2Pin, HIGH); // Enciende Co2 del acuario
      #else
        myDigitalWrite(Co2Pin, HIGH); // Enciende Co2 del acuario
      #ifdef DEBUG
      Serial.println("ha entrado en el condicional de PH y ACTIVA el Co2");
      #endif 

      #endif  
    }
  }
}

// Hasta aqui

void check_densidade()
{
  if (DEN < (setDEN + offDEN + alarmDEN) && DEN > (setDEN - offDEN - alarmDEN))
  {
    bitWrite(status_parametros, 4, 0);
  }
  if (alarmDEN > 0)           // Liga o alarme
  {
    if ((DEN >= (setDEN + offPHR + alarmDEN)) || (DEN <= (setDEN - offDEN - alarmDEN)))
    {
      bitWrite(status_parametros, 4, 1);
    }
  }
}

void check_ORP()
{
  if (ORP < (setORP + offORP + alarmORP) && ORP > (setORP - offORP - alarmORP))
  {
    bitWrite(status_parametros_1, 0, 0);
  }
  if (alarmORP > 0)            // Liga o alarme
  {
    if ((ORP >= (setORP + offORP + alarmORP)) || (ORP <= (setORP - offORP - alarmORP)))
    {
      bitWrite(status_parametros_1, 0, 1);
    }
  }

  if (outlets_changed[2] == true)
  {
    outlets_changed[2] = false;
    bitWrite(status_parametros, 7, 0);
#ifdef USE_PCF8575
    PCF8575.digitalWrite(ozonizadorPin, LOW); // Desliga ozonizador
#else
    myDigitalWrite(ozonizadorPin, LOW); // Desliga ozonizador
#endif
  }

  if (outlets[2] == 0)
  {
    if ((ORP < 100) || (ORP > 500))
    {
      bitWrite(status_parametros, 7, 0);
#ifdef USE_PCF8575
      PCF8575.digitalWrite(ozonizadorPin, LOW); // Desliga ozonizador
#else
      myDigitalWrite(ozonizadorPin, LOW); // Desliga ozonizador
#endif
    }
    else
    {
      if (ORP > setORP)
      {
        bitWrite(status_parametros, 7, 0);
#ifdef USE_PCF8575
        PCF8575.digitalWrite(ozonizadorPin, LOW);  // Desliga ozonizador
#else
        myDigitalWrite(ozonizadorPin, LOW);  // Desliga ozonizador
#endif
      }
      if (offORP > 0)
      {
        if (ORP < setORP)
        {
          bitWrite(status_parametros, 7, 1);
#ifdef USE_PCF8575
          PCF8575.digitalWrite(ozonizadorPin, HIGH); // Liga ozonizador
#else
          myDigitalWrite(ozonizadorPin, HIGH); // Liga ozonizador
#endif
        }
      }
    }
  }
  else if (outlets[2] == 1)
  {
    bitWrite(status_parametros, 7, 1);
#ifdef USE_PCF8575
    PCF8575.digitalWrite(ozonizadorPin, HIGH); // Liga ozonizador
#else
    myDigitalWrite(ozonizadorPin, HIGH); // Liga ozonizador
#endif
  }
  else if (outlets[2] == 2)
  {
    bitWrite(status_parametros, 7, 0);
#ifdef USE_PCF8575
    PCF8575.digitalWrite(ozonizadorPin, LOW); // Liga ozonizador
#else
    myDigitalWrite(ozonizadorPin, LOW); // Liga ozonizador
#endif
  }
}
void check_alarme()
{
  if ((bitRead(status_parametros, 2) == true) || (bitRead(status_parametros, 3) == true) || (bitRead(status_parametros, 4) == true) ||
      (bitRead(status_parametros, 6) == true) || (bitRead(status_parametros_1, 1) == true) || (bitRead(tpa_status, 2) == true))
  {
    myDigitalWrite (alarmPin, HIGH);
  }
  else
  {
    myDigitalWrite (alarmPin, LOW);
  }
}

void reposicao_agua_doce () // abre a solenoide 1 se o nível estiver baixo e se a tpa não estiver em andamento
//e se o chiller estiver desligado e se o nível do sump não estiver anormal e se não houve falha durante uma tpa.
{
  if (outlets_changed[8] == true)
  {
    outlets_changed[8] = false;
    bitWrite(Status, 1, 0);
#ifdef USE_PCF8575
    PCF8575.digitalWrite(solenoide1Pin, LOW);
#else
    myDigitalWrite(solenoide1Pin, LOW);
#endif
  }

  if (outlets[8] == 0)
  {
    if ((myAnalogRead(sensor3) > 400) && (myAnalogRead(sensor2) > 400) && (bitRead(tpa_status, 1) == false) && (bitRead(status_parametros, 0) == false) && (bitRead(tpa_status, 2) == false))
    {
#ifdef USE_PCF8575
      PCF8575.digitalWrite(solenoide1Pin, HIGH);
#else
      myDigitalWrite(solenoide1Pin, HIGH);
#endif
      bitWrite(Status, 1, 1); //sinaliza reposição em andamento
    }
    else
    {
#ifdef USE_PCF8575
      PCF8575.digitalWrite(solenoide1Pin, LOW);
#else
      myDigitalWrite(solenoide1Pin, LOW);
#endif
      bitWrite(Status, 1, 0);
    }
  }
  else if (outlets[8] == 1)
  {
    bitWrite(Status, 1, 1);
#ifdef USE_PCF8575
    PCF8575.digitalWrite(solenoide1Pin, HIGH);
#else
    myDigitalWrite(solenoide1Pin, HIGH);
#endif
  }
  else if (outlets[8] == 2)
  {
    bitWrite(Status, 1, 0);
#ifdef USE_PCF8575
    PCF8575.digitalWrite(solenoide1Pin, LOW);
#else
    myDigitalWrite(solenoide1Pin, LOW);
#endif
  }
}

#ifdef DISABLE_SKIMMER
void check_level_skimmer()
{
  if (outlets_changed[4] == true)
  {
    outlets_changed[4] = false;
#ifdef USE_PCF8575
    PCF8575.digitalWrite(skimmerPin, LOW);
#else
    myDigitalWrite(skimmerPin, LOW);
#endif
  }

  if (outlets[4] == 0)
  {
    if (myAnalogRead(sensor3) < 400)
    {
#ifdef USE_PCF8575
      PCF8575.digitalWrite(skimmerPin, LOW);
#else
      myDigitalWrite(skimmerPin, LOW);
#endif
    }
    else
    {
#ifdef USE_PCF8575
      PCF8575.digitalWrite(skimmerPin, HIGH);
#else
      myDigitalWrite(skimmerPin, HIGH);
#endif
    }
  }
  else if (outlets[4] == 1)
  {
#ifdef USE_PCF8575
    PCF8575.digitalWrite(skimmerPin, HIGH);
#else
    myDigitalWrite(skimmerPin, HIGH);
#endif
  }
  else if (outlets[4] == 2)
  {
#ifdef USE_PCF8575
    PCF8575.digitalWrite(skimmerPin, LOW);
#else
    myDigitalWrite(skimmerPin, LOW);
#endif
  }
}
#endif
