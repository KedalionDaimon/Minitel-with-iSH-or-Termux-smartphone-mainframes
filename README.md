# Minitel as terminal for "smartphone mainframes" (iOS & Android)

For a demonstration, see: https://youtu.be/MwfdkAkN2Ro

This project attempts to facilitate using a French Minitel terminal as a controlling terminal for a Linux-environment, either via iSH on iOS on iPhones, or via Termux on Android. This is basically very similar to the countless experiments where a lot of people try to "connect a terminal to their Linux machine, as if it were a mainframe or minicomputer", just here, the iPhone is the mainframe, so to say. As the iPhone, however, is not offering any serial port connections, an ESP32-board is involved, e.g. some clone of the WEMOS R1 D32 (which I made good experiences with), serving as a WiFi-to-Serial bridge. Flash the respective sketch - the "-server" one for "Variant 1" below, or the other one for "Variant 2" below. - As the Minitel features by nature a 5V RS232 serial port (1200,E,7,1), no special voltage regulation is needed.


## Variant 1

If the phone is accessing the WiFi hotspot on the ESP32-board:

busybox nc 192.168.4.1 8088 -e stdbuf -e0 -o0 sh -c 'socat TCP-LISTEN:8086,reuseaddr,fork EXEC:sh,pty,stderr,setsid,sigint,sane & sleep 1 && nc localhost 8086' &

(alternatively, in case of Android, you could also do:

busybox nc 192.168.4.1 8088 -e stdbuf -e0 -o0 sh -c 'busybox telnetd -p 8086 -l sh && telnet localhost 8086' 

which the iPhone does not like, and always gives you a "login" page that does not, however, log you in).


## Variant 2

If the ESP32-board is accessing the WiFI hotspot on the phone (beware to have entered your own credentials in the Arduino sketch):

socat TCP-LISTEN:8086,reuseaddr,fork EXEC:sh,pty,stderr,setsid,sigint,sane & sleep 1 && nc localhost 8086 &



Note to self: In case of issues with backspace, consider trying stty erase Ctrl-V,Ctrl-H .
