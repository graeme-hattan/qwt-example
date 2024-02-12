#ifndef WINDOW_H
#define WINDOW_H

#include <qwt/qwt_thermo.h>
#include <qwt/qwt_plot.h>
#include <qwt/qwt_plot_curve.h>

#include <QBoxLayout>
#include <QPushButton>

#include <mutex>

#include "CppTimer.h"

// class definition 'Window'
class Window : public QWidget
{
    // must include the Q_OBJECT macro for for the Qt signals/slots framework to work with this class
    Q_OBJECT


    class FakeSensor : public CppTimer {
	static constexpr double gain = 7.5;
    public:
	Window* window = nullptr;
	int count = 0;
	void timerEvent() {
	    double inVal = gain * sin( M_PI * count / 50.0 );
	    window->hasData(inVal);
	    ++count;
	}
    };

    
public:
    Window(); // default constructor - called when a Window is declared without arguments
    ~Window();

    void timerEvent( QTimerEvent * );

// internal variables for the window class
private:
    static constexpr int plotDataSize = 100;

    QPushButton  *button;
    QwtThermo    *thermo;
    QwtPlot      *plot;
    QwtPlotCurve *curve;

    

    // layout elements from Qt itself http://qt-project.org/doc/qt-4.8/classes.html
    QVBoxLayout  *vLayout;  // vertical layout
    QHBoxLayout  *hLayout;  // horizontal layout

    FakeSensor fakeSensor;

    // data arrays for the plot
    double xData[plotDataSize];
    double yData[plotDataSize];

    long count = 0;

    void reset();
    void hasData(double v);

    std::mutex mtx; 
};

#endif // WINDOW_H
