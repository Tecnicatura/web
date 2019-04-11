# Graphserial
Graph data from serial port configured

## Install
### Pre

* git https://git-scm.com/downloads

* nodejs https://nodejs.org/en/download/releases/

### Graphserial

```git clone https://github.com/hernaneche/graphserial.git```

### Dependencies

* Serialport
* Socket.io

```npm install```

### run

To change serial port number, edit gserial.html, change COM77 for whatever, and reload page

```node gserial.js```

To simulate a serial port connection

sudo socat -d -d pty,raw,echo=0,link=/dev/ttyS41 pty,raw,echo=0,link=/dev/ttyS42

despues iniciar el graphserial

sudo node gserial.js 

entrar con un screen al ttyS42 y volcar los datos ahi

sudo screen /dev/ttyS42


Binding del dongle bluetooth a rfcomm0
sudo rfcomm bind 0 98:D3:31:90:46:E4

### Browse

http://localhost/


