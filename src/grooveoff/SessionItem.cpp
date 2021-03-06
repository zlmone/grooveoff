/*
    GrooveOff - Offline Grooveshark.com music
    Copyright (C) 2013-2014  Giuseppe Calà <jiveaxe@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "SessionItem.h"
#include "ui_SessionItem.h"
#include "Utility.h"

#include <QMimeData>
#include <QDropEvent>

SessionItem::SessionItem(const QString &name, int num, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SessionItem)
{
    ui->setupUi(this);
    ui->sessionName->setText(name);
//     ui->numTracks->setText(trUtf8("%1 tracks").arg(QString::number(num)));

    m_deleteButtonEnabled = true;
    setAcceptDrops(true);

    setupUi();
    setupConnections();

    m_state = GrooveOff::FinishedState;
    stateChanged();
}

SessionItem::~SessionItem()
{
    if(ui->multiFuncButton->isCountdownStarted()) {
        ui->multiFuncButton->stopCountdown();
        removeSession();
    }

    delete ui;
}

void SessionItem::setupUi()
{
    ui->sessionName->setFont(Utility::font(QFont::Bold));

    ui->sessionName->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred); // fix hidden label
    ui->numTracks->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred); // fix hidden label

    ui->multiFuncButton->setFixedSize(QSize(Utility::buttonSize,Utility::buttonSize));

    // seems that gtk DEs use different default values than Qt...
    ui->mainLayout->setContentsMargins(4,4,4,4);
    ui->multiFuncLayout->setContentsMargins(0,4,0,4);
    ui->infoIconLayout->setContentsMargins(0,4,0,4);
    ui->infoMessageLayout->setContentsMargins(0,4,0,4);
    ui->songWidgetLayout->setContentsMargins(1,1,0,2);
    ui->songWidgetLayout->setHorizontalSpacing(5);
    ui->infoIcon->setFixedSize(QSize(Utility::buttonSize,Utility::buttonSize));
    ui->multiFuncButton->setIconSize(QSize(16,16));
}

void SessionItem::setupConnections()
{
    connect(ui->multiFuncButton, SIGNAL(clicked()),
                                 SLOT(multiFuncBtnClicked()));
    
    connect(ui->multiFuncButton, SIGNAL(countdownFinished()), 
                                 SLOT(removeSession()));
}

void SessionItem::multiFuncBtnClicked()
{
    switch(m_state) {
        case GrooveOff::FinishedState:
            if(!ui->multiFuncButton->isCountdownStarted()) {
//                 ui->multiFuncButton->setToolTip(trUtf8("Abort deletion"));
                ui->multiFuncButton->startCountdown();
            } else {
//                 ui->multiFuncButton->setToolTip(trUtf8("Delete this Session"));
                ui->multiFuncButton->stopCountdown();
                ui->multiFuncButton->setIcon(QIcon::fromTheme(QLatin1String("user-trash"),
                                              QIcon(QLatin1String(":/resources/user-trash.png"))));
            }
            break;
        default:
            // do nothing
            break;
    }
}

void SessionItem::removeSession()
{
//    m_state = GrooveOff::DeletedState;
    emit remove(sessionName());
}

void SessionItem::stateChanged()
{
    switch(m_state) {
        case GrooveOff::FinishedState:
            ui->multiFuncWidget->setVisible(false);
            ui->multiFuncButton->setIcon(QIcon::fromTheme(QLatin1String("user-trash"),
                                          QIcon(QLatin1String(":/resources/user-trash.png"))));

//             ui->multiFuncButton->setToolTip(trUtf8("Delete Session"));
            ui->infoIconWidget->setVisible(false);
            ui->infoMessageWidget->setVisible(false);
            break;
//         case GrooveOff::DeletedState:
//             ui->multiFuncWidget->setVisible(false);
//             ui->infoIcon->setPixmap(QIcon::fromTheme(QLatin1String("user-trash-full"),
//                                      QIcon(QLatin1String(":/resources/user-trash-full.png"))).pixmap(16,16));
//             ui->infoIconWidget->setVisible(true);
//             ui->infoMessageWidget->setVisible(false);
//             ui->infoMessage->setText(trUtf8("Deleted"));
//             break;
    }
}


QString SessionItem::sessionName() const
{
    return ui->sessionName->text();
}

/*!
  \brief enterEvent: manage mouse movement
  \return void
*/
void SessionItem::enterEvent(QEvent* event)
{
    switch(m_state) {
        case GrooveOff::FinishedState:
            if(m_deleteButtonEnabled)
                ui->multiFuncWidget->setVisible(true);
            break;
//         case GrooveOff::DeletedState:
//             ui->infoMessageWidget->setVisible(true);
//             break;
        default:
            // do nothing
            break;
    }

    QWidget::enterEvent(event);
}

/*!
  \brief leaveEvent: manage mouse movement
  \return void
*/
void SessionItem::leaveEvent(QEvent* event)
{
    switch(m_state) {
        case GrooveOff::FinishedState:
            if(!ui->multiFuncButton->isCountdownStarted())
                ui->multiFuncWidget->setVisible(false);
            break;
//         case GrooveOff::DeletedState:
//             ui->infoMessageWidget->setVisible(false);
//             break;
        default:
            // do nothing
            break;
    }

    QWidget::leaveEvent(event);
}

void SessionItem::deleteButtonEnabled(bool ok)
{
    m_deleteButtonEnabled = ok;
}

void SessionItem::updateNumTrack(int num)
{
//     ui->numTracks->setText(trUtf8("%1 tracks").arg(QString::number(num)));
}
