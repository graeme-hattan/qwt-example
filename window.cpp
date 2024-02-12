#include "window.h"

#include <cmath>  // for sine stuff


Window::Window()
{
    fakeSensor.window = this;
    
    // set up the thermometer
    thermo = new QwtThermo; 
    thermo->setFillBrush( QBrush(Qt::red) );
    thermo->setScale(0, 10);
    thermo->show();
    
    
    // set up the initial plot data
    for( int index=0; index<plotDataSize; ++index )
    {
	xData[index] = index;
	yData[index] = 0;
    }
    
    curve = new QwtPlotCurve;
    plot = new QwtPlot;
    // make a plot curve from the data and attach it to the plot
    curve->setSamples(xData, yData, plotDataSize);
    curve->attach(plot);
    
    plot->setAxisScale(QwtPlot::yLeft,-10,10);
    plot->replot();
    plot->show();

    button = new QPushButton("Reset");
    // see https://doc.qt.io/qt-5/signalsandslots-syntaxes.html
    connect(button,&QPushButton::clicked,this,&Window::reset);

    // set up the layout - button above thermometer
    vLayout = new QVBoxLayout();
    vLayout->addWidget(button);
    vLayout->addWidget(thermo);

    // plot to the left of button and thermometer
    hLayout = new QHBoxLayout();
    hLayout->addLayout(vLayout);
    hLayout->addWidget(plot);

    setLayout(hLayout);

    // a fake data sample every 10ms
    fakeSensor.startms(10);
    // Screen refresh every 40ms
    startTimer(40);
}

Window::~Window() {
    fakeSensor.stop();
}

void Window::reset() {
    // set up the initial plot data
    for( int index=0; index<plotDataSize; ++index )
    {
	xData[index] = index;
	yData[index] = 0;
    }
}


// add the new input to the plot
void Window::hasData(double inVal) {
    mtx.lock();
    std::move( yData, yData + plotDataSize - 1, yData+1 );
    yData[0] = inVal;
    mtx.unlock();
}

// screen refresh
void Window::timerEvent( QTimerEvent * )
{
    mtx.lock();
    curve->setSamples(xData, yData, plotDataSize);
    thermo->setValue( fabs(yData[0]) );
    mtx.unlock();
    plot->replot();
    update();
}
