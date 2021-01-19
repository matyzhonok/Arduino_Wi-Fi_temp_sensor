String millis_ToString (int millisecond){
  String strResult;
  // Выделяем миллисекунды
  strResult = String(millisecond % 1000) + " секунд";
  millisecond = millisecond / 1000;
  // Выделяем секунды
  strResult = String(millisecond % 60) + "." + strResult;
  millisecond = millisecond / 60;

  // Выделяем минуты
  if(millisecond > 0) {
    strResult = String(millisecond % 60) + " минут " + strResult;
    millisecond = millisecond / 60;
  }

  // Выделяем часы
  if(millisecond > 0) {
    strResult = String(millisecond % 24) + " часов " + strResult;
    millisecond = millisecond / 24;      
  }

  // Выделяем дни
  if(millisecond > 0) {
    strResult = String(millisecond) + ":" + strResult;    
  }  

  return strResult;
}
