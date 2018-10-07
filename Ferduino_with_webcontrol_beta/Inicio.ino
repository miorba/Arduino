#ifdef USE_TFT
//---------------------------------------Tela inicial ----------------------------------------------tela =0
int tabla_luces[] = { 9, 9, 9, 9, 9, 9, 9, 9, 9, 9 };
//tabla_luces[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
void mainScreen(boolean refreshAll = false)
{
  int ledLevel;
  int offset = 0;
  char bufferLP[16];

//Configuración iniciar de identificación de parametros activados
int luces = 0;
#ifdef LedWhite
luces++;
#endif
#ifdef LedBlue
luces++;
#endif
#ifdef LedRBlue
luces++;
#endif
#ifdef LedRed
luces++;
#endif
#ifdef LedUV
luces++;
#endif


if (luces == 1){
tabla_luces[0] = 59;
tabla_luces[1] = 183;
} else if (luces == 2) {
tabla_luces[0] = 59;
tabla_luces[1] = 116;
tabla_luces[2] = 126;
tabla_luces[3] = 183;
//Serial.print(F("entrandooo"));
//tabla_luces[] = { 59, 104, 112, 157, 0, 0, 0, 0 };
} else if (luces == 3) {
tabla_luces[0] = 59;
tabla_luces[1] = 95;
tabla_luces[2] = 103;
tabla_luces[3] = 139;
tabla_luces[4] = 147;
tabla_luces[5] = 183;
//tabla_luces[] = { 59, 87, 94, 122, 129, 157, 0, 0 };
} else if (luces == 4) {
tabla_luces[0] = 59;
tabla_luces[1] = 85;
tabla_luces[2] = 92;
tabla_luces[3] = 118;
tabla_luces[4] = 125;
tabla_luces[5] = 151;
tabla_luces[6] = 158;
tabla_luces[7] = 184;
//tabla_luces[] = { 59, 79, 85, 105, 111, 131, 137, 157 };
} else if (luces == 5) {
tabla_luces[0] = 59;
tabla_luces[1] = 79;
tabla_luces[2] = 85;
tabla_luces[3] = 105;
tabla_luces[4] = 111;
tabla_luces[5] = 131;
tabla_luces[6] = 137;
tabla_luces[7] = 157;
tabla_luces[8] = 163;
tabla_luces [9] = 183; 
}



Serial.print(F("Número de luces : "));
Serial.println(luces);
Serial.print(F("Valores de las luces : "));




for(int i = 0; i < 8; i++)
{
  Serial.println(tabla_luces[i]);
}


  setFont(SMALL, 255, 255, 0, 0, 0, 0);
  myGLCD.print(rtc.getTimeStr(FORMAT_LONG), 315, 227);

  if ((t.date != dia) || (refreshAll == true) || (t.year != ano))
  {
    dia = t.date;
    ano = t.year;
    
    drawFillRect(98, 213, 235, 226, 0, 0, 0);

    myGLCD.fillRect(10, 226, 314, 239); // Apaga fase lunar

    setFont(SMALL, 255, 255, 0, 0, 0, 0);
    myGLCD.print(rtc.getDOWStr(FORMAT_LONG), 10, 227);
    myGLCD.printNumI(t.date, 95, 227);
    myGLCD.print("DE", 115, 227);
    myGLCD.print(rtc.getMonthStr(FORMAT_LONG), 145, 227);
    myGLCD.print("DE", 225, 227);
    myGLCD.printNumI(t.year, 245, 227);

    float lunarCycle = moonPhase(t.year, t.mon, t.date); //get a value for the lunar cycle

    LP.toCharArray(bufferLP, 16);

    myGLCD.setColor(255, 255, 255);
    myGLCD.print(bufferLP, 98, 213); //Escreve descrição da fase lunar

    if ((lunarCycle * 100) < 0) //Print % of Full to LCD
    {
      myGLCD.print(" 0.00", 240, 213);
    }
    else
    {
      myGLCD.printNumF(lunarCycle * 100, 1, 240, 213);
    }

    strcpy_P(buffer, (char*)pgm_read_word_near(&(tabela_textos[181])));
    myGLCD.print(buffer, 280, 213); // tabela_textos[181] = "% ILUMINADA"

    strcpy_P(buffer, (char*)pgm_read_word_near(&(tabela_textos[182])));
    myGLCD.print(buffer, 10, 213); // tabela_textos[182] = "FASE LUNAR:"
  }

  if ( refreshAll == true)                                  //Desenha elementos fixos
  {
    myGLCD.setColor(196, 136, 17);

    strcpy_P(buffer, (char*)pgm_read_word_near(&(tabela_textos[30])));
    myGLCD.print(buffer, 50, 10); // tabela_textos[30] = "POTENCIA DOS LEDS"

    myGLCD.setColor(192, 236, 255);

    myGLCD.print("100", 24, 20);

    for (int i = 0; i < 90; i += 10)
    {
      myGLCD.printNumI(90 - i, 32, 31 + offset); // 90 até 10
      offset += 11;
    }

    myGLCD.print("0", 40, 130);

    myGLCD.setColor(255, 255, 255);

    myGLCD.drawRect(50, 138, 187, 138);       // Eixo X
    myGLCD.drawRect(50, 28, 50, 138);         // Eixo Y

    for (int i = 0; i < 10; i++)
    {
      myGLCD.drawLine(51, (i * 11) + 28, 55, (i * 11) + 28);
    } // Marcador grande
    myGLCD.setColor(190, 190, 190);

    for (int i = 0; i < 10; i++)
    {
      myGLCD.drawLine(51, (i * 11) + 34, 53, (i * 11) + 34);
    } // Marcador pequeno

    for (int i = 0; i < 10; i++)
    {
      myGLCD.drawLine(79, (i * 11) + 28, 187, (i * 11) + 28);
    }// Grade

    myGLCD.setColor(161, 127, 73);
    myGLCD.drawRect(7, 5, 199, 208);                // Desenhar retângulos
    myGLCD.drawRect(206, 5, 392, 208);
    //  myGLCD.drawRect(345, 90, 385, 185); // Atalho
  } // Fim do refreshAll
//Declaramos variables de altura
int alto = 14;
myGLCD.setColor(255, 255, 255);
    
#ifdef USE_Temp_Disp
    strcpy_P(buffer, (char*)pgm_read_word_near(&(tabela_textos[183])));
    myGLCD.print(buffer, 212, alto);  // tabela_textos[183] = "T.DISSIPADOR:"

    myGLCD.setColor(0, 255, 0);
    myGLCD.drawCircle(369, 11, 2);                  // Unidade t. dissipador

  #ifdef USE_FAHRENHEIT
    myGLCD.print("F", 372, alto);
  #else
    myGLCD.print("C", 372, alto);
  #endif
  myGLCD.setColor(0, 0, 0);
  myGLCD.fillRect(313, 14, 353, 26);                // Apagar temperatura dissipador
  myGLCD.setColor(0, 255, 0);
  myGLCD.printNumF(tempH, 2, 316, alto);   // Temperatura dissipador
alto = alto + 14;
#endif
myGLCD.setColor(255, 255, 255);
#ifdef USE_Temp_Water
    strcpy_P(buffer, (char*)pgm_read_word_near(&(tabela_textos[184])));
    myGLCD.print(buffer, 212, alto); // tabela_textos[184] = "TEMP. AGUA:"

    myGLCD.setColor(0, 255, 0);
    myGLCD.drawCircle(367, 25, 2);                  // Unidade t. água

  #ifdef USE_FAHRENHEIT
    myGLCD.print("F", 370, alto);                     // Unidade t. água
  #else
    myGLCD.print("C", 370, alto);                     // Unidade t. água
  #endif
  myGLCD.setColor(0, 0, 0);
  myGLCD.fillRect(303, 32, 320, 40);                // Apagar temperatura agua
  myGLCD.setColor(0, 255, 0);
  myGLCD.printNumF(tempC, 2, 302, alto);   // Temperatura da agua
  if (bitRead(status_parametros, 2) == true)
  {
    myGLCD.setColor(255, 0, 0);
    myGLCD.printNumF( tempC, 2, 302, alto);     // Temperatura agua em vermelho
  }
alto = alto + 14;
#endif
myGLCD.setColor(255, 255, 255);
#ifdef USE_PHAquarium
    strcpy_P(buffer, (char*)pgm_read_word_near(&(tabela_textos[185])));
    myGLCD.print(buffer, 212, alto); // tabela_textos[185] = "PH DO AQUARIO:"
    myGLCD.setColor(0, 0, 0);
    myGLCD.fillRect(324, 42, 390, 54);                // Apagar PH do aquario
    myGLCD.setColor(0, 255, 0);
    myGLCD.printNumF(PHA, 2, 327, alto);     // PH aqua
    if (bitRead(status_parametros, 3) == true)
    {
     myGLCD.setColor(255, 0, 0);
     myGLCD.printNumF(PHA, 2, 327, alto);       // PH do aquario em vermelho
    }
alto = alto + 14;
#endif
myGLCD.setColor(255, 255, 255);
#ifdef USE_PHReator
  strcpy_P(buffer, (char*)pgm_read_word_near(&(tabela_textos[186])));
  myGLCD.print(buffer, 212, alto); // tabela_textos[186] =  "PH DO REATOR:"
  myGLCD.setColor(0, 0, 0);  
  myGLCD.fillRect(318, 56, 390, 68);                // Apagar PH do reator
  myGLCD.setColor(0, 255, 0);
  myGLCD.printNumF(PHR, 2, 316, alto);    // PH reator
  if (bitRead(status_parametros, 6) == true)
  {
    myGLCD.setColor(255, 0, 0);
    myGLCD.printNumF(PHR, 2, 316, alto);       // PH do reator em vermelho
  }
alto = alto + 14;
#endif
myGLCD.setColor(255, 255, 255);
#ifdef USE_Density
    strcpy_P(buffer, (char*)pgm_read_word_near(&(tabela_textos[187])));
    myGLCD.print(buffer, 212, alto); // tabela_textos[187] = "DENSIDADE:"

    myGLCD.setColor(0, 255, 0);
    myGLCD.print("G/L", 332, alto);                   // Unidade densidade
    myGLCD.setColor(0, 0, 0);  
    myGLCD.fillRect(288, 70, 330, 82);                // Apagar densidade 
    myGLCD.setColor(0, 255, 0); 
    myGLCD.printNumI(DEN, 293, alto);        // Densidade 
    if (bitRead(status_parametros, 4) == true)
    {
     myGLCD.setColor(255, 0, 0);
     myGLCD.printNumI(DEN, 293, alto);           // Densidade em vermelho
    }
alto = alto + 14;
#endif
myGLCD.setColor(255, 255, 255);
#ifdef USE_ORP
    strcpy_P(buffer, (char*)pgm_read_word_near(&(tabela_textos[188])));
    myGLCD.print(buffer, 212, alto); // tabela_textos[188] = "ORP:"

    myGLCD.setColor(0, 255, 0);
    myGLCD.print("MV", 280, alto);                    // Unidade ORP
    myGLCD.setColor(0, 0, 0); 
    myGLCD.fillRect(245, 84, 277, 96);                // Apagar ORP
    myGLCD.setColor(0, 255, 0);
    myGLCD.printNumI(ORP, 245, alto);       // ORP
    if (bitRead(status_parametros_1, 0) == true)
    {
    myGLCD.setColor(255, 0, 0);
    myGLCD.printNumI(ORP, 245, alto);           // ORP em vermelho
    }
alto = alto + 14;
#endif
myGLCD.setColor(255, 255, 255);
#ifdef USE_Cooler
    strcpy_P(buffer, (char*)pgm_read_word_near(&(tabela_textos[189])));
    myGLCD.print(buffer, 212, alto); // tabela_textos[189] = "CHILLER:"
    myGLCD.setColor(0, 0, 0); 
    myGLCD.fillRect(279, 98, 330, 110);               // Apagar noticia de chiller ligado/desligado
    if (bitRead(status_parametros, 0) == true)
    {
     myGLCD.setColor(0, 130, 255);
     myGLCD.print("ON ", 279, alto);           // Chiller ligado
    }
    else
    {
     myGLCD.setColor(0, 255, 0);
     myGLCD.print("OFF", 279, alto);          // Chiller desligado
    }
alto = alto + 14;
#endif
myGLCD.setColor(255, 255, 255);
#ifdef USE_Hotter
    strcpy_P(buffer, (char*)pgm_read_word_near(&(tabela_textos[190])));
    myGLCD.print(buffer, 212, alto); // tabela_textos[190] = "AQUECEDOR:"
    myGLCD.setColor(0, 0, 0); 
    myGLCD.fillRect(289, 112, 330, 124);              // Apagar aviso de calentador encendido/apagado. Apagar noticia de aquecedor ligado/desligado
    if (bitRead(status_parametros, 1) == true)
    {
     myGLCD.setColor(0, 130, 255);
     myGLCD.print("ON ", 294, alto);           // Aquecedor ligado
    }
    else
    {
     myGLCD.setColor(0, 255, 0);
     myGLCD.print("OFF", 294, alto);          // Aquecedor desligado
    }
alto = alto + 14;
#endif
myGLCD.setColor(255, 255, 255);
#ifdef USE_Reator
    strcpy_P(buffer, (char*)pgm_read_word_near(&(tabela_textos[191])));
    myGLCD.print(buffer, 212, alto); // tabela_textos[191] = "REATOR:"
    myGLCD.setColor(0, 0, 0); 
    myGLCD.fillRect(270, 126, 320, 138);              // Apagar noticia de reator ligado/desligado
    if (bitRead(status_parametros, 5) == true)
    {
     myGLCD.setColor(0, 130, 255);
     myGLCD.print("ON ", 270, alto);           // Reator ligado
    }
    else
    {
     myGLCD.setColor(0, 255, 0);
     myGLCD.print("OFF", 270, alto);          // Reator desligado
    }
alto = alto + 14;
#endif
myGLCD.setColor(255, 255, 255);
#ifdef USE_Ozone
    strcpy_P(buffer, (char*)pgm_read_word_near(&(tabela_textos[192])));
    myGLCD.print(buffer, 212, alto); // tabela_textos[192] = "OZONIO:"
    myGLCD.setColor(0, 0, 0); 
    myGLCD.fillRect(270, 140, 320, 152);              // Apagar noticia de ozonio ligado/desligado
    if (bitRead(status_parametros, 7) == true)
    {
     myGLCD.setColor(0, 130, 255);
     myGLCD.print("ON ", 270, alto);           // Ozonio ligado
    }
    else
    {
     myGLCD.setColor(0, 255, 0);
     myGLCD.print("OFF", 270, alto);          // ozonio desligado
    }
alto = alto + 14;
#endif
myGLCD.setColor(255, 255, 255);
#ifdef USE_WaterRep
    strcpy_P(buffer, (char*)pgm_read_word_near(&(tabela_textos[193])));
    myGLCD.print(buffer, 212, alto); // tabela_textos[193] = "REPOSICAO:"
    myGLCD.setColor(0, 0, 0); 
    myGLCD.fillRect(289, 154, 330, 166);              // Apagar noticia de reposicao ligado/desligado
    if (bitRead(Status, 1) == true)
    {
     myGLCD.setColor(0, 130, 255);
     myGLCD.print("ON ", 294, alto);           // Reposição ligada
    }
    else
    {
     myGLCD.setColor(0, 255, 0);
     myGLCD.print("OFF", 294, alto);          // Reposição desligada
    }
alto = alto + 14;
#endif
myGLCD.setColor(255, 255, 255);
#ifdef USE_WaterLevel
    strcpy_P(buffer, (char*)pgm_read_word_near(&(tabela_textos[194])));
    myGLCD.print(buffer, 212, alto); // tabela_textos[194] = "NIVEL:"
    myGLCD.setColor(0, 0, 0);
    myGLCD.fillRect(266, 168, 330, 180);              // Apagar noticia de nivel normal/anormal 
    if (nivel_status1 == true)
    {
     myGLCD.setColor(255, 0, 0);
     strcpy_P(buffer, (char*)pgm_read_word_near(&(tabela_textos[196])));
     myGLCD.print(buffer, 265, alto);         // Nivel baixo em vermelho // tabela_textos[196] = "BAIXO!"
    }
    else
    {
     myGLCD.setColor(0, 255, 0);
     strcpy_P(buffer, (char*)pgm_read_word_near(&(tabela_textos[197])));
     myGLCD.print(buffer, 265, alto);          // Nível nornal // tabela_textos[197] = "NORMAL"
    }
alto = alto + 14;
#endif
myGLCD.setColor(255, 255, 255);
#ifdef USE_CPA
    strcpy_P(buffer, (char*)pgm_read_word_near(&(tabela_textos[195])));
    myGLCD.print(buffer, 212, alto); // tabela_textos[195] = "TPA:" // Troca de Água Parcial
    myGLCD.setColor(0, 0, 0);
    myGLCD.fillRect(250, 182, 330, 193);              // Apagar noticia de TPA ligado/desligado
    if ((bitRead(tpa_status, 1) == true) && (bitRead(tpa_status, 2) == false))
    {
     myGLCD.setColor(0, 130, 255);
     myGLCD.print("ON ", 250, alto);           // TPA em andamento
    }
    else if ((bitRead(tpa_status, 1) == false) && (bitRead(tpa_status, 2) == false))
    {
     myGLCD.setColor(0, 255, 0);
     myGLCD.print("OFF", 250, alto);           // TPA não esta em andamento
    } 
    if (bitRead(tpa_status, 2) == true)
    {
     myGLCD.setColor(255, 0, 0);
     strcpy_P(buffer, (char*)pgm_read_word_near(&(tabela_textos[86])));
     myGLCD.print(buffer, 250, alto);          // Sinaliza falha durante uma TPA // tabela_textos[86] = "FALHA!"
    }
alto = alto + 14;
#endif
myGLCD.setColor(255, 255, 255);
#ifdef USE_Temp_Ambient
    strcpy_P(buffer, (char*)pgm_read_word_near(&(tabela_textos[226])));
    myGLCD.print(buffer, 212, alto); // tabela_textos[226] = "T. AMBIENTE:"

    myGLCD.setColor(0, 255, 0);
    myGLCD.drawCircle(369, alto-3, 2);                  // Unidade t. ambiente
    
  #ifdef USE_FAHRENHEIT
    myGLCD.print("F", 370, alto);                    // Unidade t. ambiente
  #else
    myGLCD.print("C", 370, alto);                    // Unidade t. ambiente
  #endif
  myGLCD.setColor(0, 0, 0); 
  myGLCD.fillRect(312, 195, 360, 207);              // Apaga consumo de energia
  myGLCD.setColor(0, 255, 0);
  myGLCD.printNumF(tempA, 2, 312, alto);  // Temperatura ambiente
#endif

  if ((refreshAll == true) || (web_timer == true))
  {
    web_timer = false;
    if (temporizador_ativado_e[0] == 1)
    {
      myGLCD.setColor(0, 130, 255);
      strcpy_P(buffer, (char*)pgm_read_word_near(&(tabela_textos[31])));
      myGLCD.print(buffer, 12, 144); // tabela_textos[31] = "TIMER 1:"
    }
    else
    {
      myGLCD.setColor(255, 255, 255);
      strcpy_P(buffer, (char*)pgm_read_word_near(&(tabela_textos[31])));
      myGLCD.print(buffer, 12, 144); // tabela_textos[31] = "TIMER 1:"
    }

    if (temporizador_ativado_e[1] == 1)
    {
      myGLCD.setColor(0, 130, 255);
      strcpy_P(buffer, (char*)pgm_read_word_near(&(tabela_textos[77])));
      myGLCD.print(buffer, 12, 156); // tabela_textos[77] = "TIMER 2:"
    }
    else
    {
      myGLCD.setColor(255, 255, 255);
      strcpy_P(buffer, (char*)pgm_read_word_near(&(tabela_textos[77])));
      myGLCD.print(buffer, 12, 156); // tabela_textos[77] = "TIMER 2:"
    }

    if (temporizador_ativado_e[2] == 1)
    {
      myGLCD.setColor(0, 130, 255);
      strcpy_P(buffer, (char*)pgm_read_word_near(&(tabela_textos[78])));
      myGLCD.print(buffer, 12, 168); // tabela_textos[78] = "TIMER 3:"
    }
    else
    {
      myGLCD.setColor(255, 255, 255);
      strcpy_P(buffer, (char*)pgm_read_word_near(&(tabela_textos[78])));
      myGLCD.print(buffer, 12, 168); // tabela_textos[78] = "TIMER 3:"
    }

    if (temporizador_ativado_e[3] == 1)
    {
      myGLCD.setColor(0, 130, 255);
      strcpy_P(buffer, (char*)pgm_read_word_near(&(tabela_textos[79])));
      myGLCD.print(buffer, 12, 180); // tabela_textos[79] = "TIMER 4:"
    }
    else
    {
      myGLCD.setColor(255, 255, 255);
      strcpy_P(buffer, (char*)pgm_read_word_near(&(tabela_textos[79])));
      myGLCD.print(buffer, 12, 180); // tabela_textos[79] = "TIMER 4:"
    }

    if (temporizador_ativado_e[4] == 1)
    {
      myGLCD.setColor(0, 130, 255);
      strcpy_P(buffer, (char*)pgm_read_word_near(&(tabela_textos[80])));
      myGLCD.print(buffer, 12, 192); // tabela_textos[80] = "TIMER 5:"
    }
    else
    {
      myGLCD.setColor(255, 255, 255);
      strcpy_P(buffer, (char*)pgm_read_word_near(&(tabela_textos[80])));
      myGLCD.print(buffer, 12, 192); // tabela_textos[80] = "TIMER 5:"
    }
  }
int posi = 0;
#ifdef LedWhite 
  if ((whiteLed != wled_out) || refreshAll)  // Atualiza gráfico led branco
  {

    whiteLed = wled_out;
    ledLevel = LedToPercent(wled_out);

    drawFillRect(128, 144, 185, 156, 0, 0, 0);
    myGLCD.setColor(cor_canal[0][0], cor_canal[0][1], cor_canal[0][2]);

    strcpy_P(buffer, (char*)pgm_read_word_near(&(nome_canal[1])));
    myGLCD.print(buffer, 128, 144); // nome_canal[1] = "BRA"

    offset = map(ledLevel, 100, 0, 28, 137);

    myGLCD.print(":", 152, 144);
    int valorini = tabla_luces[posi];
    posi++;
    int valorfin = tabla_luces[posi];
    posi++;
    myGLCD.fillRect(valorini, offset, valorfin, 137);
//    myGLCD.fillRect(59, offset, 79, 137);
    myGLCD.print(String(ledLevel), 160, 144);
    myGLCD.print("%", 186, 144);

    drawFillRect(59, offset - 1, 79, 27, 0, 0, 0);
  }
#endif

#ifdef LedBlue 
  if ((blueLed != bled_out) || refreshAll)       // Atualiza gráfico led azul
  {
    blueLed = bled_out;
    ledLevel = LedToPercent(bled_out);

    drawFillRect(128, 156, 185, 168, 0, 0, 0);

    myGLCD.setColor(cor_canal[1][0], cor_canal[1][1], cor_canal[1][2]);

    strcpy_P(buffer, (char*)pgm_read_word_near(&(nome_canal[2])));
    myGLCD.print(buffer, 128, 156); // nome_canal[2] = "AZU"

    offset = map(ledLevel, 100, 0, 28, 137);
    myGLCD.print(":", 152, 156);
    int valorini = tabla_luces[posi];
    posi++;
    int valorfin = tabla_luces[posi];
    posi++;
    myGLCD.fillRect(valorini, offset, valorfin, 137);
//    myGLCD.fillRect(85, offset, 105, 137);
    myGLCD.print(String(ledLevel), 160, 156);
    myGLCD.print("%", 186, 156);

    drawFillRect(85, offset - 1, 105, 27, 0, 0, 0);
  }
#endif
#ifdef LedRBlue
  if ((azulroyalLed != rbled_out) || refreshAll)      // Atualiza gráfico led azul royal
  {
    azulroyalLed = rbled_out;
    ledLevel = LedToPercent(rbled_out);

    drawFillRect(128, 168, 185, 180, 0, 0, 0);

    myGLCD.setColor(cor_canal[2][0], cor_canal[2][1], cor_canal[2][2]);

    strcpy_P(buffer, (char*)pgm_read_word_near(&(nome_canal[3])));
    myGLCD.print(buffer, 128, 168); // nome_canal[3] = "AZR"

    offset = map(ledLevel, 100, 0, 28, 137);
    myGLCD.print(":", 152, 168);
    int valorini = tabla_luces[posi];
    posi++;
    int valorfin = tabla_luces[posi];
    posi++;
    myGLCD.fillRect(valorini, offset, valorfin, 137);
//    myGLCD.fillRect(111, offset, 131, 137);
    myGLCD.print(String(ledLevel), 160, 168);
    myGLCD.print("%", 186, 168);

    drawFillRect(111, offset - 1, 131, 27, 0, 0, 0);
  }
#endif
#ifdef LedRed
  if ((vermelhoLed != rled_out) || refreshAll)    // Atualiza gráfico led  vermelho
  {
    vermelhoLed = rled_out;
    ledLevel = LedToPercent(rled_out);

    drawFillRect(128, 180, 185, 192, 0, 0, 0); // Apaga potência anterior

    myGLCD.setColor(cor_canal[3][0], cor_canal[3][1], cor_canal[3][2]);

    strcpy_P(buffer, (char*)pgm_read_word_near(&(nome_canal[4])));
    myGLCD.print(buffer, 128, 180); // nome_canal[4] = "VER"

    offset = map(ledLevel, 100, 0, 28, 137);
    myGLCD.print(":", 152, 180);
    int valorini = tabla_luces[posi];
    posi++;
    int valorfin = tabla_luces[posi];
    posi++;
    myGLCD.fillRect(valorini, offset, valorfin, 137);
//    myGLCD.fillRect(137, offset, 157, 137);
    myGLCD.print(String(ledLevel), 160, 180);
    myGLCD.print("%", 186, 180);

    drawFillRect(137, offset - 1, 157, 27, 0, 0, 0); // Apaga barra anterior
  }
#endif
#ifdef LedUV
  if ((violetaLed != uvled_out) || refreshAll)      // Atualiza gráfico led violeta
  {
    violetaLed = uvled_out;
    ledLevel = LedToPercent(uvled_out);

    drawFillRect(128, 192, 185, 204, 0, 0, 0); // Apaga potência anterior

    myGLCD.setColor(cor_canal[4][0], cor_canal[4][1], cor_canal[4][2]);

    strcpy_P(buffer, (char*)pgm_read_word_near(&(nome_canal[5])));
    myGLCD.print(buffer, 128, 192); // nome_canal[5] = "VIO"

    offset = map(ledLevel, 100, 0, 28, 137);
    myGLCD.print(":", 152, 192);
    int valorini = tabla_luces[posi];
    posi++;
    int valorfin = tabla_luces[posi];
    posi++;
    myGLCD.fillRect(valorini, offset, valorfin, 137);
//    myGLCD.fillRect(163, offset, 183, 137);
    myGLCD.print(String(ledLevel), 160, 192);
    myGLCD.print("%", 186, 192);

    drawFillRect(163, offset - 1, 183, 27, 0, 0, 0); // Apaga barra anterior
  }
#endif


  myGLCD.fillRect(80, 144, 110, 156);              // Apagar noticia de Timers 1 ligado/desligado
  myGLCD.fillRect(80, 156, 110, 168);              // Apagar noticia de Timers 2 ligado/desligado
  myGLCD.fillRect(80, 168, 110, 180);              // Apagar noticia de Timers 3 ligado/desligado
  myGLCD.fillRect(80, 180, 110, 192);              // Apagar noticia de Timers 4 ligado/desligado
  myGLCD.fillRect(80, 192, 110, 204);              // Apagar noticia de Timers 5 ligado/desligado


    if (bitRead(temporizador_status, 1) == true)
  {
    myGLCD.setColor(0, 130, 255);
    myGLCD.print("ON ", 82, 144);           // Timer 1 ligado
  }
  else
  {
    myGLCD.setColor(0, 255, 0);
    myGLCD.print("OFF", 82, 144);          // Timer 1 desligado
  }

  if (bitRead(temporizador_status, 2) == true)
  {
    myGLCD.setColor(0, 130, 255);
    myGLCD.print("ON ", 82, 156);           // Timer 2 ligado
  }
  else
  {
    myGLCD.setColor(0, 255, 0);
    myGLCD.print("OFF", 82, 156);          // Timer 2 desligado
  }
  if (bitRead(temporizador_status, 3) == true)
  {
    myGLCD.setColor(0, 130, 255);
    myGLCD.print("ON ", 82, 168);           // Timer 3 ligado
  }
  else
  {
    myGLCD.setColor(0, 255, 0);
    myGLCD.print("OFF", 82, 168);          // Timer 3 desligado
  }
  if (bitRead(temporizador_status, 4) == true)
  {
    myGLCD.setColor(0, 130, 255);
    myGLCD.print("ON ", 82, 180);           // Timer 4 ligado
  }
  else
  {
    myGLCD.setColor(0, 255, 0);
    myGLCD.print("OFF", 82, 180);          // Timer 4 desligado
  }
  if (bitRead(temporizador_status, 5) == true)
  {
    myGLCD.setColor(0, 130, 255);
    myGLCD.print("ON ", 82, 192);           // Timer 5 ligado
  }
  else
  {
    myGLCD.setColor(0, 255, 0);
    myGLCD.print("OFF", 82, 192);          // Timer 5 desligado
  }
  if (temperatura_alta == true)
  {
    myGLCD.setColor(255, 0, 0);
    strcpy_P(buffer, (char*)pgm_read_word_near(&(tabela_textos[236])));// "TEMPERATURA ALTA!"
    myGLCD.print(buffer, 58, 90);          // Alerta de temperatura alta para os LEDS.
  }
  if (temperatura_baixou == true)
  {
    strcpy_P(buffer, (char*)pgm_read_word_near(&(tabela_textos[237]))); // "TEMP. BAIXOU!"
    myGLCD.setColor(0, 255, 0);
    myGLCD.print(buffer, 58, 90);          // Alerta de que a temperatura esteve alta para os LEDS.
  }
}
#endif
