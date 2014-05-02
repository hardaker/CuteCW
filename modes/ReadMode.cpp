#include <qdebug.h>

#include <QFileDialog>
#include <QMessageBox>
#include <QFormLayout>
#include <QtCore/QTimer>
#include <QMenu>
#include <QtXml/QDomDocument>
#include <QTextDocumentFragment>

#include "ReadMode.h"
#include "Morse.h"

ReadMode::ReadMode(Morse *parent, Ui::MainWindow *ui)
    : MorseMode(parent, ui), m_textEdit(0), m_readWordCount(1), m_mapper(new QSignalMapper()), m_oldText()
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
ReadMode::switchFromMode() {
    m_readSpot = m_textEdit->textCursor();
    m_readSpot.select(QTextCursor::Document);
    m_oldText = m_readSpot.selectedText();
}

void
ReadMode::addButtons() {
    QPushButton *button = new QPushButton(tr("Load File"));
    QVBoxLayout *vLayout = new QVBoxLayout();
    QVBoxLayout *buttonLayout = new QVBoxLayout();
    m_ui->forModes->addLayout(vLayout);

    QHBoxLayout *hLayout = new QHBoxLayout();
    vLayout->addLayout(hLayout);
    hLayout->addLayout(buttonLayout);
    m_textEdit = new QTextEdit();
    m_textEdit->setText(m_oldText);
    hLayout->addWidget(m_textEdit);

    buttonLayout->addWidget(button);
    connect(button, SIGNAL(clicked()), this, SLOT(openFile()));

    button = new QPushButton("Get News");
    QMenu *menu = new QMenu(button);
    button->setMenu(menu);
    buttonLayout->addWidget(button);

    QAction *action;

    connect(action = menu->addAction("ARRL - Ham News"), SIGNAL(triggered()), m_mapper, SLOT(map()));
    m_mapper->setMapping(action, "http://www.arrl.org/arrl.rss");

    // fails for some reason I haven't dived into yet:
    //connect(action = menu->addAction("EHAM - Postings"), SIGNAL(triggered()), m_mapper, SLOT(map()));
    //m_mapper->setMapping(action, "http://www.eham.net/rss/rss.php");

    connect(action = menu->addAction("SlashDot - News for Nerds"), SIGNAL(triggered()), m_mapper, SLOT(map()));
    m_mapper->setMapping(action, "http://rss.slashdot.org/Slashdot/slashdot");

    connect(action = menu->addAction("BBC - North America"), SIGNAL(triggered()), m_mapper, SLOT(map()));
    m_mapper->setMapping(action, "http://feeds.bbci.co.uk/news/world/us_and_canada/rss.xml");

    connect(action = menu->addAction("CNN - Top Stories"), SIGNAL(triggered()), m_mapper, SLOT(map()));
    m_mapper->setMapping(action, "http://rss.cnn.com/rss/cnn_topstories.rss");

    connect(action = menu->addAction("Word of the Day"), SIGNAL(triggered()), m_mapper, SLOT(map()));
    m_mapper->setMapping(action, "http://wordsmith.org/awad/rss1.xml");

    connect(action = menu->addAction("Quote of the Day"), SIGNAL(triggered()), m_mapper, SLOT(map()));
    m_mapper->setMapping(action, "http://feeds.feedburner.com/quotationspage/qotd");

    connect(m_mapper, SIGNAL(mapped(QString)), this, SLOT(fetchNews(QString)));

    setupWPMWidgets(vLayout);
}

void
ReadMode::fetchNews(const QString &source) {
    qDebug() << "loading: " << source;
    m_manager = new QNetworkAccessManager(this);
    m_reply = m_manager->get(QNetworkRequest(QUrl(source)));
    if (m_reply) {
        connect(m_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(netLoadFinished(QNetworkReply*)));
        m_textEdit->setText("Loading news....  Please wait....");
    }
}

void
ReadMode::netLoadFinished(QNetworkReply *reply) {
    if (reply->error() != QNetworkReply::NoError) {
        m_textEdit->setText("Failed to load the news");
        return;
    }
    QDomDocument document;
    QTextDocumentFragment fragment;
    if (document.setContent(reply)) {
        QDomNodeList nodes = document.elementsByTagName("title");
        QDomNodeList storyNodes = document.elementsByTagName("description");
        if (!nodes.isEmpty()) {
            QString totalText;
            for (int i = 1; i < storyNodes.count(); i++) { // skip the first title: is the whole RSS title
                QDomNode node = nodes.item(i+1);
                QDomNode story = storyNodes.item(i);
                totalText = totalText + node.firstChild().nodeValue() + "\n";
                fragment = QTextDocumentFragment::fromHtml(story.firstChild().nodeValue());
                qDebug() << "here: " << fragment.toPlainText();
                totalText = totalText + fragment.toPlainText() + "\n\n";
            }
            m_textEdit->setText(totalText);
        } else {
            m_textEdit->setText("no news is likely not good news in this case");
        }
    } else {
        m_textEdit->setText("Failed to load the news document");
    }
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
    if (m_readSpot.atEnd())
        m_readSpot.movePosition(QTextCursor::Start);
    readWordUnderCursor();
    m_currentWordCount = 1;
    return;
}

void ReadMode::stop() {
    if (m_textEdit && !m_readSpot.isNull()) {
        m_textEdit->setTextCursor(m_readSpot);
        m_readSpot.select(QTextCursor::WordUnderCursor);
        m_readSpot.insertHtml(m_currentText);
    }
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
    // decide if we're going to re-read the current word or not.
    if (m_currentWordCount >= m_readWordCount) {
        m_currentWordCount = 1;
        m_readSpot.select(QTextCursor::WordUnderCursor);
        m_readSpot.insertHtml(m_currentText);
        m_currentText = "";
        m_readSpot.movePosition(QTextCursor::NextWord);
    } else {
        m_currentWordCount++;
    }

    if (!m_readSpot.atEnd()) {
        QTimer::singleShot(int(m_morse->spaceSecsF() * 1000.0), this, SLOT(readWordUnderCursor()));
    } else {
        m_textEdit->setTextCursor(m_readSpot);
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

QString ReadMode::icon()
{
    return ":/icons/64x64/read-to-me.png";
}


void ReadMode::loadSettings(QSettings &settings)
{
    m_readWordCount = settings.value("readtome/readWordCount", 1).toInt();
}

void ReadMode::saveSettings(QSettings &settings)
{
    settings.setValue("readtome/readWordCount", m_readWordCount);
}

QBoxLayout *ReadMode::getPrefsLayout()
{
    QHBoxLayout *hbox = new QHBoxLayout();
    QFormLayout *form = new QFormLayout();
    hbox->addLayout(form);

    form->addRow(tr("Read each word this many times:"), m_readWordCountBox = new QSpinBox());
    m_readWordCountBox->setRange(1,10);
    m_readWordCountBox->setValue(m_readWordCount);

    return hbox;
}

void ReadMode::acceptPrefs()
{
    m_readWordCount = m_readWordCountBox->value();
}
