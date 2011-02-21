#include <qdebug.h>

#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>

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
    m_readSpot.select(QTextCursor::Document);

    m_morse->add(m_readSpot.selectedText());
    m_morse->playSequence();
    return;
    //qDebug() << "starting to read";
    //m_readSpot = m_ui->wordbox->cursorForPosition(QPoint(0,0));
    //readNextLetter();
}

void ReadMode::readNextLetter() {
    if (m_readSpot.atEnd()) {
        qDebug() << "reached the end; stopping playing";
        return;
    }
    // move the anchor to where the cursor is
    m_readSpot.movePosition(QTextCursor::NoMove, QTextCursor::MoveAnchor, 0);
    // move the selection pointer to the right one, highlighting the current
    // selection letter we're going to play
    m_readSpot.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, 1);
    // play the selection
    qDebug() << "playing selected text: " << m_readSpot.selectedText();
    m_morse->addAndPlayIt(m_readSpot.selectedText()[0]);
}

void ReadMode::audioStopped()
{
    qDebug() << "audio stopped method";
    setRunningMode(PAUSED);
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

