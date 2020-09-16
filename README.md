# QtRTSP

RTSP example to read IP camera using Qt 5.15.  Works with Android using ffmpeg_android_maker in Linux but only in Release mode.  Crashes in Debug mode because no debug DLLs are present.

Works with Windows with ffmpeg from https://ffmpeg.zeranoe.com/builds/ and Linux.

https://github.com/Javernaut/ffmpeg-android-maker

Edit QtRTSP.json file to add up to 6 RTSP streams.  App likely crashes if stream not available.
