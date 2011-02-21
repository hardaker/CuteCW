#include <qdebug.h>

#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>
#include <QtCore/QTimer>

#include "ReadMode.h"
#include "Morse.h"

ReadMode::ReadMode(Morse *parent, Ui::MainWindow *ui)
    : MorseMode(parent, ui)
{
}

void
ReadMode::switchToMode() {
    m_ui->helpBar->setText("<font color=\"green\">Enter text and hit the play button to hear the entire sequence.</font>");
    m_ui->play->show();

    clear();
    addButtons();
}

void
ReadMode::addButtons() {
    QPushButton *button = new QPushButton(tr("Load File"));
    QVBoxLayout *vLayout = new QVBoxLayout();
    m_ui->forModes->addLayout(vLayout);

    QHBoxLayout *hLayout = new QHBoxLayout();
    vLayout->addLayout(hLayout);
    hLayout->addWidget(button);
    m_textEdit = new QTextEdit();
    hLayout->addWidget(m_textEdit);
    connect(button, SIGNAL(clicked()), this, SLOT(openFile()));

    setupWPMWidgets(vLayout);
}

void
ReadMode::openFile() {
    QString fileName = QFileDialog::getOpenFileName(0, tr("Load a file"));
    if (fileName.isEmpty())
        return;
    QFile file(fileName);
    file.open(QFile::ReadOnly);
    QString contents = file.readAll();
    if (contents.isEmpty()) {
        QMessageBox::critical(0, tr("Failed to read file"), tr("Opening file %1 failed").arg(fileName));
        return;
    }
    m_textEdit->setText(contents);
}

void ReadMode::play() {
    m_readSpot = m_textEdit->textCursor();
    m_readSpot.movePosition(QTextCursor::Start);
    readWordUnderCursor();

    return;
}

void ReadMode::stop() {
    m_readSpot.movePosition(QTextCursor::End);
}

void ReadMode::readWordUnderCursor() {
    m_readSpot.select(QTextCursor::WordUnderCursor);
    m_morse->add(m_readSpot.selectedText());
    m_morse->playSequence();
    QTextCharFormat format = m_readSpot.blockCharFormat();
    format.setFontUnderline(true);
    //m_readSpot.setBlockCharFormat(format);
    m_currentText = m_readSpot.selectedText();
    m_readSpot.insertHtml("<u>" + m_currentText + "</u>");
}

void ReadMode::audioStopped()
{
    m_readSpot.select(QTextCursor::WordUnderCursor);
    m_readSpot.insertHtml(m_currentText);
    m_currentText = "";
    m_readSpot.movePosition(QTextCursor::NextWord);
    if (!m_readSpot.atEnd()) {
        QTimer::singleShot(int(m_morse->spaceSecs() * 1000.0), this, SLOT(readWordUnderCursor()));
    } else {
        qDebug() << "audio stopped method";
        setRunningMode(PAUSED);
    }
}

QString ReadMode::helpText()
{
    return tr("Learning by hearing a stream of text can be difficult at first.  Start with simple books like children would start with, like 'Cat in the Hat', and "
              "only after mastering listening to those simple books with short words should you move to difficult texts like Shakespere or technical documents."
              "<p>To listen to text more slowly, try to leave the WPM rate set to your goal (we suggest 20) and vary only the Letter and Word spacing.  "
              "The word spacing affects the pauses between the words, and the letter spacing affects the pauses between the letters.  "
              "Start by decreasing both if you need to, but try and get back to full speed letter spacing first and then word spacing. "
              "<p><b>BUG WARNING:</b> do not load big files...  A bug will end up using way too much memory.");
}

QString ReadMode::name()
{
    return tr("Read to me");
}

