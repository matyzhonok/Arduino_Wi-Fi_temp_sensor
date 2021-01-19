void print_to_web(String str, WiFiClient client, boolean need_gui) {
  if (need_gui) {
    String str_head = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<HTML>\r\n<HEAD>\r\n<meta charset='utf-8'>\r\n</HEAD>\r\n<BODY>\r\n";
    String str_ending = "\r\n</BODY>\r\n</HTML>";
    str = str_head + str + str_ending;
    client.print(str);
  } else {
    String str_head = "HTTP/1.1 200 OK\r\nContent-Type: text/json\r\n\r\n";
    str = str_head + str;
    client.print(str);    
  }
  
}
