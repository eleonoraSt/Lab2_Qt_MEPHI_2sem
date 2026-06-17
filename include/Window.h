#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QRadioButton>
#include <QCheckBox>
#include <QTextEdit>
#include <QLabel>
#include <QString>

#include <string>
#include <sstream>
#include <cmath>

#include <iostream>  // отладка

#include "ArraySequence.h"
#include "ListSequence.h"
#include "ImmutableArraySequence.h"
#include "ImmutableListSequence.h"
#include "AdaptiveSequence.h"
#include "BitSequence.h"

#define WINDOW_HEIGHT 560
#define LABEL_FIELD_CHAR_LEN 15
#define BUF_ALLOCATE 30

enum SeqMode {
    INT_MODE,
    FLOAT_MODE,
    BIT_MODE
};

class Window: public QWidget {
    Q_OBJECT
public:
    explicit Window(QWidget *parent = nullptr);
private slots:
    void createSequence();
    void getFirstButtonClicked();
    void getLastButtonClicked();
    void getButtonClicked();
    void getSubseqButtonClicked();
    void getLengthButtonClicked();
    void appendButtonClicked();
    void prependButtonClicked();
    void insertButtonClicked();
    void switchModeButtonClicked(bool checked);
    void deviationButtonClicked();
    void appClosed();
private:
    QRadioButton* ListSequenceButton;
    QRadioButton* ArraySequenceButton;
    QRadioButton* ImmutableListSequenceButton;
    QRadioButton* ImmutableArraySequenceButton;
    QRadioButton* AdaptiveSequenceButton;
    QRadioButton* BitSequenceButton;

    QCheckBox* floatButton;

    QTextEdit* inputField;

    QPushButton* confirmButton;

    QLabel* instruction;
    QLabel* errorMessage;

    QPushButton* GetFirstButton;
    QPushButton* GetLastButton;
    QPushButton* GetButton;
    QPushButton* GetSubseqButton;
    QPushButton* GetLengthButton;
    QPushButton* AppendButton;
    QPushButton* PrependButton;
    QPushButton* InsertButton;
    QPushButton* SwitchModeButton;
    QPushButton* DeviationButton;

    QTextEdit* GetFirstField;
    QTextEdit* GetLastField;
    QTextEdit* GetIndex;
    QTextEdit* GetField;
    QTextEdit* GetSubseqStart;
    QTextEdit* GetSubseqEnd;
    QTextEdit* GetLengthField;
    QTextEdit* AppendField;
    QTextEdit* PrependField;
    QTextEdit* InsertField;
    QTextEdit* InsertIndex;
    QTextEdit* DeviationField;

    Sequence<int>* intSeq;
    Sequence<float>* floatSeq;
    BitSequence* bitSeq;
    SeqMode mode;
    bool isAdaptive;
    AdaptiveSeqMode adaptiveMode;

    //float sigmaSquare;

    template <class T> T* ReadInputField(int* writeSize) {
        std::string input = inputField->toPlainText().toStdString() + " ";  // чтобы не рассматривать отдельный случай для конца
        std::istringstream inputStream;
        inputStream.str(input);
        T* array = new T[BUF_ALLOCATE];
        T* arrayBuf;
        int index = 0, pos = 0;
        T buf;

        inputStream >> buf;
        pos = inputStream.tellg();
        while (pos != -1) {
            if (index > 0 && index % BUF_ALLOCATE == 0) {  // достигнут конец выделенной памяти
                arrayBuf = array;
                array = new T[index + BUF_ALLOCATE];
                delete [] arrayBuf;
            }
            array[index] = buf;
            index++;
            inputStream >> buf;
            pos = inputStream.tellg();
        }
        *writeSize = index;
        return array;
    }

    static float squaresSum(float x, float y) {
        return x * x + y;
    }

    static float square(float x) {
        return x * x;
    }
};

#endif // WINDOW_H
