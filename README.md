# Boo Schedule View README

This software broadcasts admission schedule of an office (primarily designed for polyclinic) by RTSP-protocol as a sequence of pictures. This enables showing the schedule by smart-TV's connected to the local area network.

Software consists of two essential parts:

* Server which actually multicasts the picture,

* Client - for managing the schedules.

## Details

Content of the schedule is stored in a MySQL (or MariaDB) database. The Boo Server acquires this information, groups it, splits by pages and forms pictures. How long each peace of information (a screenful) is being shown, color scheme, column titles of the schedule tables and other options could be set up by yourself.

The frames' succession is than being compressed, packed to RTSP and delivered to the clients by multicast (if the network allows multicasting). Gstreamer is used to compose, H264-encode and stream the video. H264 encoder runs with zerolatency quality which is not very good for movies but makes a good option for live-streaming and loads the computer not very much. Default video resolution is 640x480 with framerate of 10 per second but it'll be fine to set resolution to greater values. Increasing framerate gives not much quality (since the video is mostly a static picture) but significally drops system perfomance.

Boo Client is a small and user-friendly GUI application which allows editing the schedules. The client may be (or should be) installed on a different computer than the server. Changes made by the client are available to the server on the next server's request to the database to generate an image of the page you have edited.

## Server Installation

No binary for the Boo Server is available, you have to compile it yourself.

### Building on Linux

To build the Boo Server on Linux you'll need GCC, cmake and development packages for:

* gstreamer, gstreamer-video, gstreamer-app, gstreamer-rtsp-server;

* freetype;

* mysqlclient.

Use cmake to build booserver.

### Building on Windows

Running the Boo Server on Windows is generally not a good idea but for development purpose.

For building the server on Windows better option is to use CodeLite IDE. Open the booserver.workspace file from within CodeLite. You will probably need to change the paths to the libaries and include files:

* gstreamer and plugins;

* freetype;

* MYSQL Connector C 6.1.

### Database Setup

1. Create users

2. Import booserver.sql

### Server Setup

On Linux host there should be installed freetype, mysql (or mariadb), gstreamer, gstreamer-video, gstreamer-app, gstreamer-rtsp-server, gst-plugins-good, gst-plugins-bad, gst-plugins-ugly. The last one sounds awful but contains x264video codec which can't be helped.

Copy configuration files (booserver/\*.conf) and fonts (\*.ttf, \*.otf) to the same location as the binary. Change mysql connection parameters and network interface in booserver.conf in accordance to your need. You can change other options also.

Make sure the font you selected in the configuration file exists in the same directory as the server's binary.

### Starting Server

The server should be started from the directory it's located in. On Linux:

```
./booserver
```

On Windows the command should be something like:

```
booserver.exe --gst-root=C:\gstreamer\1.0\x86 --gst-plugin-path=C:\gstreamer\1.0\x86\lib\gstreamer-1.0
```

If everything is good server prints URL to connect to and waits for connections.

### Testing

Assuming you are testing on a computer from which the server is available by network. Start VLC Media Player and select "Media/Open URL..." (Ctrl+N). Enter the URL the booserver has shown you and click the play button. In a couple of seconds you should see the first page of the schedule.

If nothing is shown it is possible gstreamer failed to found some plugin. Try to stop booserver and run it with debug option to see what's wrong:

```
./booserver --gst-debug=1
```
