#include "controlpanel.h"

ControlPanel::ControlPanel()
{
    setupLayout();
    //buildConnection();
}

ControlPanel::ControlPanel(SlideGLWidget * canvas)
{
    setupLayout();
    this -> canvas = canvas;
    buildConnection();
}

void ControlPanel::buildConnection()
{
    /* Build our connections. */
    connect(viewContent, SIGNAL(activated(int)), canvas, SLOT(viewContentChanged(int)));
    connect(mergeButton, SIGNAL(clicked(bool)), this, SLOT(test(bool)));
    connect(subdivLevelSlider,SIGNAL(valueChanged(int)), canvas, SLOT(levelChanged(int)));
    connect(subdivLevelSlider, SIGNAL(valueChanged(int)), this, SLOT(viewContentSetToSubdiv(int)));
    connect(resetViewButton, SIGNAL(clicked(bool)), canvas, SLOT(resetViewDirection(bool)));
    connect(minOffsetBox, SIGNAL(textChanged(QString)), this, SLOT(resetMinOffset(QString)));
    connect(maxOffsetBox, SIGNAL(textChanged(QString)), this, SLOT(resetMaxOffset(QString)));
    connect(offsetStepBox, SIGNAL(textChanged(QString)), this, SLOT(resetOffsetStep(QString)));
    connect(offsetValueSlider, SIGNAL(valueChanged(int)), this, SLOT(offSetSliderMoved(int)));
    connect(this, SIGNAL(makeOffsetMesh(float)), canvas, SLOT(offsetValueChanged(float)));

}

void ControlPanel::setupLayout()
{
    /* Main Layout.
     * Contains view, mode, merge, subdivision, offset, color.*/
    setLayout(mainLayout = new QVBoxLayout);
    mainLayout -> addLayout(viewLayout = new QVBoxLayout);
    mainLayout -> addLayout(modeLayout = new QVBoxLayout);
    mainLayout -> addLayout(mergeLayout = new QVBoxLayout);
    mainLayout -> addLayout(subdivLayout = new QVBoxLayout);
    mainLayout -> addLayout(offsetLayout = new QVBoxLayout);
    mainLayout -> addLayout(colorLayout = new QVBoxLayout);
    /* View layout. */
    viewLayout -> addWidget(new QLabel("View Panel"));
    viewLayout -> addWidget(new QLabel("View Content:"));
    viewLayout -> addWidget(viewContent = new QComboBox);
    viewContent -> addItem("Hirachical Scene");
    viewContent -> addItem("Initial Mesh");
    viewContent -> addItem("Subdivision Mesh");
    viewContent -> addItem("Offset Mesh");
    viewContent -> addItem("Subdivision on Offset Mesh");
    viewContent -> setCurrentIndex(1);
    viewLayout -> addWidget(resetViewButton = new QPushButton(tr("Reset View")));
    /* Mode layout.*/
    modeLayout -> addWidget(new QLabel("Mode Panel"));
    /* Merge layout. */
    mergeLayout -> addWidget(mergeButton = new QPushButton(tr("Merge")));
    /* Subdivision layout. */
    subdivLayout -> addWidget(new QLabel("Subvidision Panel"));
    subdivLayout -> addLayout(subdivLevelLayout = new QHBoxLayout);
    subdivLevelLayout -> addWidget(new QLabel("Level 0"));
    subdivLevelLayout -> addWidget(subdivLevelSlider = new QSlider(Qt::Horizontal));
    subdivLevelLayout -> addWidget(new QLabel("Level 5"));
    subdivLayout -> addWidget(currentLevelLabel = new QLabel("Current Subdivision Level:    0"));
    subdivLevelSlider -> setMinimum(0);
    subdivLevelSlider -> setMaximum(5);
    subdivLevelSlider -> setValue(0);
    /* Offset Layout. */
    maxOffset = 0.11;
    minOffset = 0.01;
    offsetStep = 2;
    offsetLayout -> addWidget(new QLabel("Offset Panel"));
    offsetLayout -> addLayout(offsetMinMaxLayout = new QHBoxLayout);
    offsetMinMaxLayout -> addWidget((new QLabel(tr("Min"))));
    offsetMinMaxLayout -> addWidget(minOffsetBox = new QLineEdit(QString::number(minOffset)));
    offsetMinMaxLayout -> addWidget((new QLabel(tr("Max"))));
    offsetMinMaxLayout -> addWidget(maxOffsetBox = new QLineEdit(QString::number(maxOffset)));
    offsetMinMaxLayout -> addWidget((new QLabel(tr("Step"))));
    offsetMinMaxLayout -> addWidget(offsetStepBox = new QLineEdit(QString::number(offsetStep)));
    offsetLayout -> addLayout(offsetValueLayout = new QHBoxLayout);
    offsetValueLayout -> addWidget(offsetValueSlider = new QSlider(Qt::Horizontal));
    offsetValueSlider -> setMinimum(0);
    offsetValueSlider -> setMaximum(offsetStep);
    offsetValueSlider -> setValue(0);
    /* Color Layout. */
    colorLayout -> addWidget(new QLabel("Color Panel"));
    colorLayout -> addLayout(foreColorLayout = new QHBoxLayout);
    colorLayout -> addLayout(backColorLayout = new QHBoxLayout);
    foreColorLayout -> addWidget(foreColorButton = new QPushButton(tr("Foreground Color")));
    foreColorLayout -> addWidget(foreColorBox = new QWidget());
    foreColorBox -> resize(5,5);
    QPalette forePal = foreColorBox->palette();
    forePal.setColor(foreColorBox->backgroundRole(), QColor(255,0,0));
    foreColorBox->setPalette(forePal);
    foreColorBox -> setAutoFillBackground(true);
    backColorLayout -> addWidget(backColorButton = new QPushButton(tr("Background Color")));
    backColorLayout -> addWidget(backColorBox = new QWidget());
    QPalette backPal = foreColorBox->palette();
    backPal.setColor(backColorBox->backgroundRole(), QColor(0,0,0));
    backColorBox->setPalette(backPal);
    backColorBox -> setAutoFillBackground(true);
    backColorBox -> resize(5,5);
}

void ControlPanel::test(QString test)
{
    cout<<test.toStdString()<<endl;
}

void ControlPanel::test(bool checked)
{
    cout<<"Hello!"<<endl;
}

void ControlPanel::viewContentReset()
{
    viewContent ->setCurrentIndex(1);
}

void ControlPanel::viewContentSetToSubdiv(int level)
{
    viewContent -> setCurrentIndex(2);
    currentLevelLabel -> setText(tr("Current Subdivision Level:    ") + QString::number(level));
}

void ControlPanel::viewContentSetToOffset()
{
    viewContent -> setCurrentIndex(3);
}

void ControlPanel::resetMinOffset(QString minOffset)
{
    this->minOffset = minOffset.toFloat();
}

void ControlPanel::resetMaxOffset(QString maxOffset)
{
    this->maxOffset = maxOffset.toFloat();
}

void ControlPanel::resetOffsetStep(QString offsetStep)
{
    this->offsetStep = offsetStep.toInt();
    offsetValueSlider->setMaximum(this->offsetStep);
}

void ControlPanel::offSetSliderMoved(int value)
{
    float realOffsetValue = minOffset + (maxOffset - minOffset) / offsetStep * value;
    emit makeOffsetMesh(realOffsetValue);
}

void ControlPanel::viewContentSetToSubdivOffset()
{
    viewContent -> setCurrentIndex(4);
}
