# QtRTSP

RTSP example to read IP camera using Qt 5.15.  Works with Android using ffmpeg_android_maker, Linux and Windows.  May need to copy DLLs from Windows ffmpeg bin folder to app folder.

Works with Windows with ffmpeg from https://ffmpeg.zeranoe.com/builds/ and ffmpeg in Linux.

https://github.com/Javernaut/ffmpeg-android-maker

Edit QtRTSP.json file to add up to 6 RTSP streams.  App likely crashes if stream not available.  QtRTSP.json likely found in ~/.local/QtRTSP in Linux, c:\users\{user}\Appdata\Local\QtRTSP in Windows and {root}/QtRTSP folder in Android.  A default file is written if QtRTSP.json is not found.
