#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

class Background : public QWidget
{
public:
    Background(QWidget * parent = NULL);
    int setBackground(QString & imagePath);
    int clearBackground();
};

#endif /* BACKGROUND_HPP */
