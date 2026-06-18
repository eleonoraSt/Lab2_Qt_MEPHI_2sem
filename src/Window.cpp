#include <QApplication>

#include "..\include\Window.h"
#include "..\include\ArraySequence.h"
#include "..\include\ListSequence.h"
#include "..\include\ImmutableArraySequence.h"
#include "..\include\ImmutableListSequence.h"
#include "..\include\AdaptiveSequence.h"

Window::Window(QWidget* parent) {
    ListSequenceButton = new QRadioButton("Двусвязный список", this);
    ListSequenceButton->setGeometry(10, 10, 300, 20);
    ArraySequenceButton = new QRadioButton("Динамический массив", this);
    ArraySequenceButton->setGeometry(10, 40, 300, 20);
    ImmutableListSequenceButton = new QRadioButton("Неизменяемый двусвязный список", this);
    ImmutableListSequenceButton->setGeometry(10, 70, 300, 20);
    ImmutableArraySequenceButton = new QRadioButton("Неизменяемый динамический массив", this);
    ImmutableArraySequenceButton->setGeometry(10, 100, 300, 20);
    AdaptiveSequenceButton = new QRadioButton("Адаптивная последовательность", this);
    AdaptiveSequenceButton->setGeometry(10, 130, 300, 20);
    BitSequenceButton = new QRadioButton("Битовый динамический массив", this);
    BitSequenceButton->setGeometry(10, 160, 300, 20);

    floatButton = new QCheckBox("Разрешить дроби", this);
    floatButton->setGeometry(10, 190, 300, 20);

    inputField = new QTextEdit(this);
    inputField->setGeometry(10, 220, 300, 300);

    confirmButton = new QPushButton("Подтвердить", this);
    confirmButton->setGeometry(10, 530, 80, 30);

    instruction = new QLabel(this);
    instruction->setText("Введите числа, либо только 1 и 0 (для битовой последовательности).");
    instruction->setGeometry(10, 570, 400, 20);
    errorMessage = new QLabel(this);
    errorMessage->setText("");  // По умолчанию ошибки нет
    errorMessage->setGeometry(10, 590, 400, 20);

    GetFirstButton = new QPushButton("Первый элемент", this);
    GetFirstButton->setGeometry(320, 10, 150, 20);
    GetLastButton = new QPushButton("Последний элемент", this);
    GetLastButton->setGeometry(320, 40, 150, 20);
    GetButton = new QPushButton("Элемент №", this);
    GetButton->setGeometry(320, 70, 150, 20);
    GetSubseqButton = new QPushButton("Срез", this);
    GetSubseqButton->setGeometry(320, 100, 150, 20);
    GetLengthButton = new QPushButton("Длина", this);
    GetLengthButton->setGeometry(320, 130, 150, 20);
    AppendButton = new QPushButton("Добавить в конец", this);
    AppendButton->setGeometry(320, 160, 150, 20);
    PrependButton = new QPushButton("Добавить в начало", this);
    PrependButton->setGeometry(320, 190, 150, 20);
    InsertButton = new QPushButton("Вставить по индексу", this);
    InsertButton->setGeometry(320, 220, 150, 20);
    SwitchModeButton = new QPushButton("С массива на список", this);
    SwitchModeButton->setGeometry(320, 250, 150, 20);
    SwitchModeButton->setCheckable(true);
    SwitchModeButton->setVisible(false);
    DeviationButton = new QPushButton("Отклонение", this);
    DeviationButton->setGeometry(320, 280, 150, 20);

    GetFirstField = new QTextEdit(this);
    GetFirstField->setGeometry(480, 10, 50, 20);
    GetFirstField->setReadOnly(true);
    GetLastField = new QTextEdit(this);
    GetLastField->setGeometry(480, 40, 50, 20);
    GetLastField->setReadOnly(true);
    GetIndex = new QTextEdit(this);
    GetIndex->setGeometry(480, 70, 50, 20);
    GetField = new QTextEdit(this);
    GetField->setGeometry(540, 70, 50, 20);
    GetField->setReadOnly(true);
    GetSubseqStart = new QTextEdit(this);
    GetSubseqStart->setGeometry(480, 100, 50, 20);
    GetSubseqEnd = new QTextEdit(this);
    GetSubseqEnd->setGeometry(540, 100, 50, 20);
    GetLengthField = new QTextEdit(this);
    GetLengthField->setGeometry(480, 130, 50, 20);
    GetLengthField->setReadOnly(true);
    AppendField = new QTextEdit(this);
    AppendField->setGeometry(480, 160, 50, 20);
    PrependField = new QTextEdit(this);
    PrependField->setGeometry(480, 190, 50, 20);
    InsertField = new QTextEdit(this);
    InsertField->setGeometry(480, 220, 50, 20);
    InsertIndex = new QTextEdit(this);
    InsertIndex->setGeometry(540, 220, 50, 20);
    DeviationField = new QTextEdit(this);
    DeviationField->setGeometry(320, 310, 300, 210);
    DeviationField->setReadOnly(true);

    intSeq = new ListSequence<int>();  // Чтобы по умолчанию интерпретировалось как пустой список
    floatSeq = nullptr;
    bitSeq = nullptr;
    mode = INT_MODE;
    isAdaptive = false;
    adaptiveMode = ARRAY;

    //sigmaSquare = 0;

    connect(confirmButton, SIGNAL (clicked()), this, SLOT (createSequence()));
    connect(GetFirstButton, SIGNAL (clicked()), this, SLOT (getFirstButtonClicked()));
    connect(GetLastButton, SIGNAL (clicked()), this, SLOT (getLastButtonClicked()));
    connect(GetButton, SIGNAL (clicked()), this, SLOT (getButtonClicked()));
    connect(GetSubseqButton, SIGNAL (clicked()), this, SLOT (getSubseqButtonClicked()));
    connect(GetLengthButton, SIGNAL (clicked()), this, SLOT (getLengthButtonClicked()));
    connect(AppendButton, SIGNAL (clicked()), this, SLOT (appendButtonClicked()));
    connect(PrependButton, SIGNAL (clicked()), this, SLOT (prependButtonClicked()));
    connect(InsertButton, SIGNAL (clicked()), this, SLOT (insertButtonClicked()));
    connect(SwitchModeButton, SIGNAL (clicked(bool)), this, SLOT (switchModeButtonClicked(bool)));
    connect(DeviationButton, SIGNAL (clicked()), this, SLOT (deviationButtonClicked()));
    connect(QApplication::instance(), SIGNAL (aboutToQuit()), this, SLOT (appClosed()));
}

void Window::createSequence() {
    int size;
    mode = floatButton->isChecked() ? FLOAT_MODE : INT_MODE;  // Проверка на BIT_MODE ниже

    if (floatSeq) {
        delete floatSeq;
        floatSeq = nullptr;
    }
    if (intSeq) {
        delete intSeq;
        intSeq = nullptr;
    }
    if (bitSeq) {
        delete bitSeq;
        bitSeq = nullptr;
    }
    if (ListSequenceButton->isChecked()) {
        if (floatButton->isChecked()) {
            float* array = ReadInputField<float>(&size);
            floatSeq = new ListSequence<float>(array, size);
            delete [] array;
        } else {
            int* array = ReadInputField<int>(&size);
            for (int i = 0; i < size; i++) {
                std::cout << array[i] << "\n";
            }
            intSeq = new ListSequence<int>(array, size);
            delete [] array;
        }
        isAdaptive = false;
        SwitchModeButton->setVisible(false);
    }
    if (ArraySequenceButton->isChecked()) {
        if (floatButton->isChecked()) {
            float* array = ReadInputField<float>(&size);
            floatSeq = new ArraySequence<float>(array, size);
            delete [] array;
        } else {
            int* array = ReadInputField<int>(&size);
            intSeq = new ArraySequence<int>(array, size);
            delete [] array;
        }
        isAdaptive = false;
        SwitchModeButton->setVisible(false);
    }
    if (ImmutableListSequenceButton->isChecked()) {
        if (floatButton->isChecked()) {
            float* array = ReadInputField<float>(&size);
            floatSeq = new ImmutableListSequence<float>(array, size);
            delete [] array;
        } else {
            int* array = ReadInputField<int>(&size);
            intSeq = new ImmutableListSequence<int>(array, size);
            delete [] array;
        }
        isAdaptive = false;
        SwitchModeButton->setVisible(false);
    }
    if (ImmutableArraySequenceButton->isChecked()) {
        if (floatButton->isChecked()) {
            float* array = ReadInputField<float>(&size);
            floatSeq = new ImmutableArraySequence<float>(array, size);
            delete [] array;
        } else {
            int* array = ReadInputField<int>(&size);
            intSeq = new ImmutableArraySequence<int>(array, size);
            delete [] array;
        }
        isAdaptive = false;
        SwitchModeButton->setVisible(false);
    }
    if (AdaptiveSequenceButton->isChecked()) {
        std::cout << "it truly is adaptive\n";
        if (floatButton->isChecked()) {
            float* array = ReadInputField<float>(&size);
            floatSeq = new AdaptiveSequence<float>(array, size, ARRAY);
            delete [] array;
        } else {
            int* array = ReadInputField<int>(&size);
            intSeq = new AdaptiveSequence<int>(array, size, ARRAY);
            delete [] array;
        }
        isAdaptive = true;
        adaptiveMode = ARRAY;
        SwitchModeButton->setVisible(true);
    }
    if (BitSequenceButton->isChecked()) {
        bool* array = ReadInputField<bool>(&size);
        bitSeq = new BitSequence(array, size);
        isAdaptive = false;
        SwitchModeButton->setVisible(false);
        mode = BIT_MODE;
    }
}

void Window::getFirstButtonClicked() {
    std::string getFirstRes;
    try {
        if (mode == INT_MODE) {
            getFirstRes = std::to_string(intSeq->GetFirst());
        } else if (mode == FLOAT_MODE) {
            getFirstRes = std::to_string(floatSeq->GetFirst());
        } else {
            getFirstRes = std::to_string(bitSeq->GetFirst());
        }
        GetFirstField->setText(QString::fromStdString(getFirstRes));
    } catch (ErrorCode error) {
        errorMessage->setText("Пустой список");
    }
}

void Window::getLastButtonClicked() {
    std::string getLastRes;
    try {
        if (mode == INT_MODE) {
            getLastRes = std::to_string(intSeq->GetLast());
        } else if (mode == FLOAT_MODE) {
            getLastRes = std::to_string(floatSeq->GetLast());
        } else {
            getLastRes = std::to_string(bitSeq->GetLast());
        }
        GetLastField->setText(QString::fromStdString(getLastRes));
    } catch (ErrorCode error) {
        errorMessage->setText("Пустой список");
    }
}

void Window::getButtonClicked() {
    std::string getRes;
    int index;
    try {
        index = std::stoi(GetIndex->toPlainText().toStdString()) - 1;
    } catch (std::invalid_argument error) {
        errorMessage->setText("Введите натуральное число");
        return;
    }
    try {
        if (mode == INT_MODE) {
            getRes = std::to_string(intSeq->Get(index));
        } else if (mode == FLOAT_MODE) {
            getRes = std::to_string(floatSeq->Get(index));
        } else {
            getRes = std::to_string(bitSeq->Get(index));
        }
        GetField->setText(QString::fromStdString(getRes));
    } catch (ErrorCode error) {
        errorMessage->setText("Адрес вне допустимого диапазона");
    }
}

void Window::getSubseqButtonClicked() {
    int start, end;
    try {
        start = std::stoi(GetSubseqStart->toPlainText().toStdString()) - 1;
        end = std::stoi(GetSubseqEnd->toPlainText().toStdString()) - 1;
    } catch (std::invalid_argument error) {
        errorMessage->setText("Введите натуральное число");
        return;
    }
    try {
        if (mode == INT_MODE) {
            Sequence<int>* subseq = intSeq->GetSubsequence(start, end);
            delete intSeq;
            intSeq = subseq;
        } else if (mode == FLOAT_MODE) {
            Sequence<float>* subseq = floatSeq->GetSubsequence(start, end);
            delete floatSeq;
            floatSeq = subseq;
        } else {
            BitSequence* subseq = bitSeq->GetSubsequence(start, end);
            delete bitSeq;
            bitSeq = subseq;
        }
    } catch (ErrorCode error) {
        errorMessage->setText("Адреса вне допустимого диапазона");
    }
}

void Window::getLengthButtonClicked() {
    int length;
    if (mode == INT_MODE) {
        length = intSeq->GetLength();
    } else if (mode == FLOAT_MODE) {
        length = floatSeq->GetLength();
    } else {
        length = bitSeq->GetLength();
    }
    GetLengthField->setText(QString::fromStdString(std::to_string(length)));
}

void Window::appendButtonClicked() {
    try {
        if (mode == INT_MODE) {
            int elem = stoi(AppendField->toPlainText().toStdString());
            Sequence<int>* bufSeq = intSeq->Append(elem);
            if (bufSeq != intSeq) {  // Immutable
                delete intSeq;
                intSeq = bufSeq;
            }
        } else if (mode == FLOAT_MODE) {
            float elem = stof(AppendField->toPlainText().toStdString());
            Sequence<float>* bufSeq = floatSeq->Append(elem);
            if (bufSeq != floatSeq) {  // Immutable
                delete floatSeq;
                floatSeq = bufSeq;
            }
        } else {
            bool elem = AppendField->toPlainText().toStdString().at(0) != '0';
            bitSeq->Append(elem);
        }
    } catch (std::invalid_argument error) {
        errorMessage->setText("Введите валидный элемент");
    } catch (std::out_of_range error) {
        errorMessage->setText("Введите элемент");
    }
}

void Window::prependButtonClicked() {
    try {
        if (mode == INT_MODE) {
            int elem = stoi(PrependField->toPlainText().toStdString());
            Sequence<int>* bufSeq = intSeq->Prepend(elem);
            if (bufSeq != intSeq) {  // Immutable
                delete intSeq;
                intSeq = bufSeq;
            }
        } else if (mode == FLOAT_MODE) {
            float elem = stof(PrependField->toPlainText().toStdString());
            Sequence<float>* bufSeq = floatSeq->Prepend(elem);
            if (bufSeq != floatSeq) {  // Immutable
                delete floatSeq;
                floatSeq = bufSeq;
            }
        } else {
            bool elem = PrependField->toPlainText().toStdString().at(0) != '0';
            bitSeq->Prepend(elem);
        }
    } catch (std::invalid_argument error) {
        errorMessage->setText("Введите валидный элемент");
    } catch (std::out_of_range error) {
        errorMessage->setText("Введите элемент");
    }
}

void Window::insertButtonClicked() {
    int index;
    try {
        index = std::stoi(InsertIndex->toPlainText().toStdString()) - 1;
    } catch (std::invalid_argument error) {
        errorMessage->setText("Введите натуральное число в поле адреса (второе поле)");
        return;
    }
    try {
        if (mode == INT_MODE) {
            int elem = stoi(InsertField->toPlainText().toStdString());
            Sequence<int>* bufSeq = intSeq->InsertAt(elem, index);
            if (bufSeq != intSeq) {  // Immutable
                delete intSeq;
                intSeq = bufSeq;
            }
        } else if (mode == FLOAT_MODE) {
            float elem = stof(InsertField->toPlainText().toStdString());
            Sequence<float>* bufSeq = floatSeq->InsertAt(elem, index);
            if (bufSeq != floatSeq) {  // Immutable
                delete floatSeq;
                floatSeq = bufSeq;
            }
        } else {
            bool elem = InsertField->toPlainText().toStdString().at(0) != '0';
            bitSeq->InsertAt(elem, index);
        }
    } catch (std::invalid_argument error) {
        errorMessage->setText("Введите валидный элемент");
    } catch (std::out_of_range error) {
        errorMessage->setText("Введите элемент");
    } catch (ErrorCode error) {
        errorMessage->setText("Адрес вне допустимого диапазона");
    }
}

void Window::switchModeButtonClicked(bool checked) {
    if (mode == INT_MODE) {
        AdaptiveSequence<int>* adaptiveSeq = (AdaptiveSequence<int>*)intSeq;
        adaptiveSeq->SwitchMode();
    } else {
        AdaptiveSequence<float>* adaptiveSeq = (AdaptiveSequence<float>*)intSeq;
        adaptiveSeq->SwitchMode();
    }
    if (checked) {
        SwitchModeButton->setText("Со списка на массив");
    } else {
        SwitchModeButton->setText("С массива на список");
    }
}

void Window::deviationButtonClicked() {
    if (mode != FLOAT_MODE) {
        errorMessage->setText("Функция определена только для дробей");
        return;
    }
    float sigmaSquare = floatSeq->Reduce(squaresSum, 0);
    sigmaSquare /= floatSeq->GetLength();
    int size = floatSeq->GetLength();
    float elem;
    std::ostringstream outputString;
    for (int index = 0; index < size; index++) {
        elem = floatSeq->Get(index);
        elem = sigmaSquare - elem * elem;
        if (elem < 0) elem = -elem;
        outputString << std::sqrt(elem) << " ";
    }
    DeviationField->setText(QString::fromStdString(outputString.str()));
}

void Window::appClosed() {
    if (floatSeq) {
        delete floatSeq;
    }
    if (intSeq) {
        delete intSeq;
    }
    if (bitSeq) {
        delete bitSeq;
    }
}
