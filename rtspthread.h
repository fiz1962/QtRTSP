#ifndef RTSPTHREAD_H
#define RTSPTHREAD_H

#include <QThread>
#include <QGraphicsView>
#include <string>

using namespace std;

extern "C" {
#include <libavutil/opt.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}

class RTSPThread : public QThread
{
    Q_OBJECT
public:
    RTSPThread();
    virtual ~RTSPThread();

    bool Init(QGraphicsView* thisView, string thisURL);
    void Close();
    void SaveFrame(AVFrame *pFrame, int width, int height);

    QGraphicsView* view;
    string url;
    QGraphicsScene* scene;

    AVFormatContext *pFormatCtx;
    AVDictionary* options;
    AVCodec* pCodec;
    AVCodecContext* pCodecCtx;
    AVPacket packet;
    AVFrame* pFrame;
    AVFrame *pFrameRGB;
    struct SwsContext *sws_ctx;
    int videoStream;
    uint8_t *buffer;
    int numBytes;

protected:
    void run();
};

#endif // RTSPTHREAD_H
