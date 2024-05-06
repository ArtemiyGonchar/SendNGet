# SendNGet

*Share any files with any user!*

##Client-side using

**Connection page**
To start sharing or receiving files, you must connect to host first. On this page you can set host's IP and port.

**Main page**
Here you can see all connected users. They have UUID like you. You can select any user and set any file to send. When button send pressed - receiver will get "ask page".

**Ask page**
If someone will send you file, you will see ask page. Here you can see file size, file name, and UUID of sender. You can choose between two buttons - yes or no. If you want to get file - press yes, if dont - press no.

**About sending**
When you choose file to send, file will be sended by chunks. Also for recognizing what is file name, bytes, etc. client have mini protocol.

##Server

When user connected, server will send all connected clients via json. When user sharing some files - server read them from socket and sending to receiver. SERVER DOESNT SAVE ANY DATA.