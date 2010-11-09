#include <qdebug.h>

#include "ReadMode.h"
#include "Morse.h"

ReadMode::ReadMode(Morse *parent, Ui::MainWindow *ui)
    : MorseMode(parent, ui)
{
}

void
ReadMode::switchToMode() {
    m_ui->wordbox->show();
    m_ui->modeMenu->setText("Read to me!");
    m_ui->helpBar->setText("<font color=\"green\">Enter text and hit the play button to hear the entire sequence.</font>");
    m_ui->play->show();
    clear();
}


void ReadMode::play() {
    QTextCursor readSpot = m_ui->wordbox->textCursor();
    readSpot.select(QTextCursor::Document);
    m_morse->add(readSpot.selectedText());
    m_morse->playSequence();
    return;
    qDebug() << "starting to read";
    m_readSpot = m_ui->wordbox->cursorForPosition(QPoint(0,0));
    readNextLetter();
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
