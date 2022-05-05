#include <qdebug.h>
#include <QMenu>
#include <QRandomGenerator>

#include "WordTrainingMode.h"

WordTrainingMode::WordTrainingMode(Morse *parent, Ui::MainWindow *ui)
    : MorseMode(parent, ui)
{
    setupWords();
}


void WordTrainingMode::setupWords() {
#include "words/100words.h"
#include "words/200words.h"
#include "words/300words.h"
#include "words/400words.h"
#include "words/500words.h"
}

void WordTrainingMode::setupWordsMenu() {

    m_wordsNumber = N100;

    m_wordSignalMapper = new QSignalMapper();
    QMenu *modeMenu = new QMenu(m_ui->changeWords);
    m_ui->changeWords->setMenu(modeMenu);

    QAction *action = modeMenu->addAction("Words 1-100");
    connect(action, SIGNAL(triggered()), m_wordSignalMapper, SLOT(map()));
    m_wordSignalMapper->setMapping(action, (int) N100);

    action = modeMenu->addAction("Words 101-200");
    connect(action, SIGNAL(triggered()), m_wordSignalMapper, SLOT(map()));
    m_wordSignalMapper->setMapping(action, (int) N200);

    action = modeMenu->addAction("Words 201-300");
    connect(action, SIGNAL(triggered()), m_wordSignalMapper, SLOT(map()));
    m_wordSignalMapper->setMapping(action, (int) N300);

    action = modeMenu->addAction("Words 301-400");
    connect(action, SIGNAL(triggered()), m_wordSignalMapper, SLOT(map()));
    m_wordSignalMapper->setMapping(action, (int) N400);

    action = modeMenu->addAction("Words 401-500");
    connect(action, SIGNAL(triggered()), m_wordSignalMapper, SLOT(map()));
    m_wordSignalMapper->setMapping(action, (int) N500);

    connect(m_wordSignalMapper, SIGNAL(mapped(int)), this, SLOT(switchWords(int)));
}

void WordTrainingMode::switchToMode() {
    setupSequenceLayout();

    m_ui->letter->show();
    m_ui->changeWords->show();
    m_ui->helpBar->setText("<font color=\"green\">Enter the word you hear and hit enter.</font>");
    m_ui->play->show();

    setupWordsMenu();
    setupKeyWidgets("abcdefghijklmnopqrstuvwxyz");
    setSequenceText();
}

void WordTrainingMode::switchWords(int sequence) {
    m_wordsNumber = (wordNums) sequence;
    qDebug() << "switching to: " << m_wordsNumber;
}

void WordTrainingMode::play() {
    enterPressed();
}

bool WordTrainingMode::enterPressed() {
    m_wordnumber = QRandomGenerator::global()->generate()%(m_maxWord);
    m_morse->add((*(words[m_wordsNumber]))[m_wordnumber]);
    m_morse->maybePlaySequence(true);
    m_enteredWord = "";
    m_ui->letter->setText("");
    m_wordWasGood = true;
    m_rightCount = 0;
    setSequenceText();
    return true;
}

void WordTrainingMode::setSequenceText()
{
    m_sequenceLabel->setText(tr("Words: %1/%2").arg(m_maxWord).arg(words[m_wordsNumber]->length()));
}

void WordTrainingMode::handleKeyPress(QChar letter) {
    if (runningMode() != RUNNING)
        return;
    if (letter == '\r') {
        enterPressed();
        return;
    }

    if ((*(words[m_wordsNumber]))[m_wordnumber].length() == m_enteredWord.length()) // they already hit the length previously
        return;

    if ((*(words[m_wordsNumber]))[m_wordnumber][m_enteredWord.length()] == letter) {
        m_ui->letter->setText(m_ui->letter->text() + "<font color=\"green\">" + letter + "<font>");
        m_rightCount++;
    } else {
        m_ui->letter->setText(m_ui->letter->text() + "<font color=\"red\">" + letter + "<font>");
        m_wordWasGood = false;
    }
    m_enteredWord.append(letter);
    if ((*(words[m_wordsNumber]))[m_wordnumber].length() == m_enteredWord.length()) {
        if (m_wordWasGood) {
            m_ui->letter->setText(m_ui->letter->text() + " - <font color=\"green\">GOOD</font>");
            if (m_maxWord < 10)
                m_maxWord += 2;
            else
                m_maxWord += 1;
            if (m_maxWord > (*(words[m_wordsNumber])).count())
                m_maxWord = (*(words[m_wordsNumber])).count();

        } else {
            m_ui->letter->setText(m_ui->letter->text() + " - <font color=\"red\">FAIL (" + (*(words[m_wordsNumber]))[m_wordnumber] + ")</font>");
            if (m_maxWord > 1)
                m_maxWord--;
        }
        emit groupEntered(m_rightCount, m_enteredWord.length());
    }
}

QString WordTrainingMode::helpText()
{
    return
            tr("Most words that you read or hear are from a very small set of words.  For example, 33% of the most common words fall into 100 simple words.  "
              "If you memorize these words so you can regonize them not as individual letters but as entire words you'll greatly increase your listening rate.  "
              "<p>As you hear words, type them in and hit enter when done.  After hitting enter the next word will be played.  "
               "As you begin the number of words selected from will be small but will increase quickly for every right answer (and decrease for every wrong)."
               "<p>Once you get good at the words in this group, try switching to the <i>Word Game</i> mode and see what score you can achieve!");
}

QString WordTrainingMode::name()
{
    return tr("Word Training");
}

QString WordTrainingMode::icon()
{
    return ":/icons/64x64/words.png";
}

void WordTrainingMode::loadSettings(QSettings &settings)
{
    QString prefix = name();
    m_wordsNumber = (wordNums) settings.value(prefix + "/wordsNumber",  int(N100)).toInt();
    m_maxWord     =            settings.value(prefix + "/maxWord",      2).toInt();
}

void WordTrainingMode::saveSettings(QSettings &settings)
{
    QString prefix = name();
    settings.setValue(prefix + "/wordsNumber", m_wordsNumber);
    settings.setValue(prefix + "/maxWord",     m_maxWord);
}
