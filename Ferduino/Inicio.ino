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
    #ifdef USECO2
    
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