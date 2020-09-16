#include "rtspthread.h"
#include <QDebug>

RTSPThread::RTSPThread()
{
    pFormatCtx = NULL;
    options = NULL;
    av_dict_set(&options, "rtsp_transport", "tcp", 0);
    videoStream = 0;
    sws_ctx = NULL;
    buffer = NULL;
}

RTSPThread::~RTSPThread()
{
    delete scene;
}

void RTSPThread::run()
{
    int n=0;

    while( av_read_frame(pFormatCtx, &packet) >= 0 )  {
        if(packet.stream_index==videoStream) {

            /// Decode video frame
            //avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);

            // Did we get a video frame?
            int ret = avcodec_send_packet(pCodecCtx, &packet);
            if( ret < 0 )
                qDebug() << "Error sending packet";

            ret = avcodec_receive_frame(pCodecCtx, pFrame);
            //qDebug() << "ret " << ret;
            if(ret >= 0) {
                sws_scale(sws_ctx, (uint8_t const * const *)pFrame->data,
                  pFrame->linesize, 0, pCodecCtx->height,
                  pFrameRGB->data, pFrameRGB->linesize);

                  qDebug() << "Frame finished";
                SaveFrame(pFrameRGB, pCodecCtx->width, pCodecCtx->height);
            }
        }

        //av_free_packet(&packet);
        av_packet_unref(&packet);

    }
}

bool RTSPThread::Init(QGraphicsView *thisView, string thisURL) {
    url = thisURL;
    view = thisView;
    scene = new QGraphicsScene;
    view->setScene(scene);

    if (avformat_open_input(&pFormatCtx, thisURL.c_str(), NULL, &options) != 0) {
        qDebug() << "ERROR could not open the file";
        return false;
    }

    if(avformat_find_stream_info(pFormatCtx, NULL)<0) {
        qDebug() << "Could not find stream info?";
        return false;
    }

    pCodec=avcodec_find_decoder(pFormatCtx->streams[videoStream]->codecpar->codec_id);
    pCodecCtx=avcodec_alloc_context3(pCodec);
    avcodec_parameters_to_context(pCodecCtx, pFormatCtx->streams[videoStream]->codecpar);
    avcodec_open2(pCodecCtx, pCodec,NULL);

    av_init_packet(&packet);
    pFrame = av_frame_alloc();
    if (!pFrame) {
        qDebug() << "Error allocating the frame";
        return false;
    }

    sws_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,
                             pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_RGB24,
                             SWS_BILINEAR, NULL, NULL, NULL);

    pFrameRGB = NULL;
    pFrameRGB=av_frame_alloc();
    if(pFrameRGB==NULL) {
        qDebug() << "Could not alloc pFrameRGB";
        return false;
    }

    numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height, 1);
    buffer=(uint8_t *)av_malloc(numBytes*sizeof(uint8_t));

    //avpicture_fill((AVPicture *)pFrameRGB, buffer, AV_PIX_FMT_RGB24,
    //                pCodecCtx->width, pCodecCtx->height);
    av_image_fill_arrays (pFrameRGB->data, pFrameRGB->linesize, buffer, AV_PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height, 1);

    return true;
}

void RTSPThread::SaveFrame(AVFrame *pFrame, int width, int height) {
  QPixmap pixmap(height, width);
  QByteArray pixData;
  pixData.append(QString("P6 %1 %2 255 ").arg(width).arg(height));
  pixData.append((char *)pFrame->data[0], width * height * 3);
  pixmap.loadFromData((uchar *)pixData.data(), pixData.size());
  pixmap = pixmap.scaledToWidth(view->width()*0.75);
  delete scene;
  scene = new QGraphicsScene;
  scene->addPixmap(pixmap);
  view->setScene(scene);
  qDebug() << "Updating stream";
  //update();
  //QApplication::processEvents();
}

void RTSPThread::Close() {
    avcodec_close(pCodecCtx);
    avformat_close_input(&pFormatCtx);
    qDebug() << "Stream closed";
}
