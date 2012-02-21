#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

#ifdef WINDOWS
#define SCREEN_WIDTH   (800)
#define SCREEN_HEIGHT  (450)
#else
#define SCREEN_WIDTH   (1080)
#define SCREEN_HEIGHT  (720)
#endif

class Background : public QWidget
{
public:
    Background(QWidget * parent = NULL);
    int setBackground(QString & imagePath);
    int clearBackground();
};

#endif /* BACKGROUND_HPP */
