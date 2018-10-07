void readPh()
{
  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  static float pHValue, voltage;
  if(millis()-samplingTime > samplingInterval)
  {
      pHArray[pHArrayIndex++]=analogRead(SensorPin);
      if(pHArrayIndex==ArrayLenth)pHArrayIndex=0;
      voltage = avergearray(pHArray, ArrayLenth)*5.0/1024;
//      pHValue = 3.5*voltage+Offset;
      if (phaloop<31) {
        valorpha = valorpha + (-5.70*voltage+Offset+CalibracionPHA+21.34); 
          #ifdef DEBUG
          Serial.print("El valor recogido de PH es: ");
          Serial.println(-5.70*voltage+Offset+CalibracionPHA+21.34);
          Serial.print("El valor acumulado de PH es: ");
          Serial.println(valorpha);
          Serial.print("El número de pasada para lectura PH es; ");
          Serial.println(phaloop);
          #endif
        phaloop=phaloop+1;
        if (phaloop==31){
          PHA = valorpha / 30;
          phaloop = 1;
          valorpha = 0;
          #ifdef DEBUG
          Serial.print("El valor PH final es: ");
          Serial.println(PHA);
          #endif
        }
      }
      //PHA = 3.5*voltage+Offset+CalibracionPHA;
      samplingTime=millis();
  }
  if(millis() - printTime > printInterval)   //Every 800 milliseconds, print a numerical, convert the state of the LED indicator
  {
//  Serial.print("Voltage:");
//        Serial.print(voltage,2);
//        Serial.print("    pH value: ");
//  Serial.println(PHA,2);
//        digitalWrite(LED,digitalRead(LED)^1);
//        printTime=millis();
//  PHA = phValue;
  }
}
double avergearray(int* arr, int number){
  int i;
  int max,min;
  double avg;
  long amount=0;
  if(number<=0){
    Serial.println("Error number for the array to avraging!/n");
    return 0;
  }
  if(number<5){   //less than 5, calculated directly statistics
    for(i=0;i<number;i++){
      amount+=arr[i];
    }
    avg = amount/number;
    return avg;
  }else{
    if(arr[0]<arr[1]){
      min = arr[0];max=arr[1];
    }
    else{
      min=arr[1];max=arr[0];
    }
    for(i=2;i<number;i++){
      if(arr[i]<min){
        amount+=min;        //arr<min
        min=arr[i];
      }else {
        if(arr[i]>max){
          amount+=max;    //arr>max
          max=arr[i];
        }else{
          amount+=arr[i]; //min<=arr<=max
        }
      }//if
    }//for
    avg = (double)amount/(number-2);
  }//if
  return avg;
}
