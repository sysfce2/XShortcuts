/* Copyright (c) 2021-2024 hors<horsicq@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "xshortcutswidget.h"

XShortcutsWidget::XShortcutsWidget(QWidget *pParent) : QWidget(pParent)
{
    g_pShortcuts = &g_scEmpty;
    g_pXOptions = &g_xOptionsEmpty;
    g_bIsActive = false;
    g_bIsReadonly = false;
}

XShortcutsWidget::~XShortcutsWidget()
{
#ifdef QT_DEBUG
    if (g_pXOptions == &g_xOptionsEmpty) {
        qDebug("NO OPTIONS: %s", this->objectName().toLatin1().data());
    }
#endif
}

void XShortcutsWidget::setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions)
{
    g_pShortcuts = pShortcuts;
    g_pXOptions = pXOptions;

    if (g_bIsActive) {
        reloadShortcuts();
    }

    adjustView();
}

XShortcuts *XShortcutsWidget::getShortcuts()
{
    return g_pShortcuts;
}

XOptions *XShortcutsWidget::getGlobalOptions()
{
    // #ifdef QT_DEBUG
    //     qDebug("getGlobalOptions: %X", (unsigned long long)g_pXOptions);
    // #endif
    return g_pXOptions;
}

void XShortcutsWidget::setActive(bool bState)
{
    g_bIsActive = bState;
}

void XShortcutsWidget::saveTableModel(QAbstractItemModel *pModel, const QString &sFileName)
{
    if (pModel) {
        QString _sFileName = QFileDialog::getSaveFileName(this, tr("Save"), sFileName, QString("%1 (*.txt);;%2 (*)").arg(tr("Text files"), tr("All files")));

        if (!_sFileName.isEmpty()) {
            if (!XOptions::saveTableModel(pModel, _sFileName)) {
                QMessageBox::critical(XOptions::getMainWidget(this), tr("Error"), QString("%1: %2").arg(tr("Cannot save file"), _sFileName));
            }
        }
    }
}

void XShortcutsWidget::saveTreeModel(QAbstractItemModel *pModel, const QString &sFileName)
{
    QString _sFileName = QFileDialog::getSaveFileName(this, tr("Save"), sFileName, QString("%1 (*.txt);;%2 (*)").arg(tr("Text files"), tr("All files")));

    if (!_sFileName.isEmpty()) {
        if (!XOptions::saveTreeModel(pModel, _sFileName)) {
            QMessageBox::critical(XOptions::getMainWidget(this), tr("Error"), QString("%1: %2").arg(tr("Cannot save file"), _sFileName));
        }
    }
}

void XShortcutsWidget::saveTextEdit(QTextEdit *pTextEdit, const QString &sFileName)
{
    QString _sFileName = QFileDialog::getSaveFileName(this, tr("Save"), sFileName, QString("%1 (*.txt);;%2 (*)").arg(tr("Text files"), tr("All files")));

    if (!_sFileName.isEmpty()) {
        if (!XOptions::saveTextEdit(pTextEdit, _sFileName)) {
            QMessageBox::critical(XOptions::getMainWidget(this), tr("Error"), QString("%1: %2").arg(tr("Cannot save file"), _sFileName));
        }
    }
}

void XShortcutsWidget::adjustViewChildren(QWidget *pWidget)
{
    QList<XShortcutsWidget *> listWidgets = pWidget->findChildren<XShortcutsWidget *>();

    qint32 nNumberOfWidgets = listWidgets.count();

    for (qint32 i = 0; i < nNumberOfWidgets; i++) {
        XShortcutsWidget *pChild = dynamic_cast<XShortcutsWidget *>(listWidgets.at(i));

        if (pChild) {
            pChild->adjustView();
        }
    }
}

void XShortcutsWidget::setGlobalChildren(QWidget *pWidget, XShortcuts *pShortcuts, XOptions *pXOptions)
{
    QList<XShortcutsWidget *> listWidgets = pWidget->findChildren<XShortcutsWidget *>();

    qint32 nNumberOfWidgets = listWidgets.count();

    for (qint32 i = 0; i < nNumberOfWidgets; i++) {
        XShortcutsWidget *pChild = dynamic_cast<XShortcutsWidget *>(listWidgets.at(i));

        if (pChild) {
            pChild->setGlobal(pShortcuts, pXOptions);
        }
    }
}

bool XShortcutsWidget::eventFilter(QObject *pObj, QEvent *pEvent)
{
    Q_UNUSED(pObj)

    if (pEvent->type() == QEvent::FocusIn) {
        g_bIsActive = true;
        reloadShortcuts();
    } else if (pEvent->type() == QEvent::FocusOut) {
        g_bIsActive = false;
        registerShortcuts(false);
    }

    return QWidget::eventFilter(pObj, pEvent);
}

QString XShortcutsWidget::getOpenFileName(const QString &sDirectory)
{
    return QFileDialog::getOpenFileName(this, tr("Open file") + QString("..."), sDirectory, tr("All files") + QString(" (*)"));
}

void XShortcutsWidget::_blockSignals(QObject **ppObjects, qint32 nCount, bool bState)
{
    for (qint32 i = 0; i < nCount; i++) {
        if (ppObjects[i]) {
            ppObjects[i]->blockSignals(bState);
        }
    }
}

void XShortcutsWidget::reloadShortcuts()
{
    registerShortcuts(false);
    registerShortcuts(true);
}

void XShortcutsWidget::setWidgetFocus()
{
}

void XShortcutsWidget::deleteOldAbstractModel(QAbstractItemModel **g_ppOldModel)
{
    // #if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    //     QFuture<void> future=QtConcurrent::run(&XShortcutsWidget::_deleteOldAbstractModel,this,g_ppOldModel);
    // #else
    //     QFuture<void> future=QtConcurrent::run(this,&XShortcutsWidget::_deleteOldAbstractModel,g_ppOldModel);
    // #endif

    //    return future;

    _deleteOldAbstractModel(g_ppOldModel);
}

void XShortcutsWidget::setReadonly(bool bState)
{
    g_bIsReadonly = bState;
}

bool XShortcutsWidget::isReadonly()
{
    return g_bIsReadonly;
}

void XShortcutsWidget::reloadData(bool bSaveSelection)
{
    Q_UNUSED(bSaveSelection)
#ifdef QT_DEBUG
    qDebug("reloadData");
#endif
}

void XShortcutsWidget::setLocation(quint64 nLocation, qint32 nLocationType, qint64 nSize)
{
    Q_UNUSED(nLocation)
    Q_UNUSED(nLocationType)
    Q_UNUSED(nSize)
#ifdef QT_DEBUG
    qDebug("setLocation");
#endif
}
// #ifdef QT_CONCURRENT_LIB
// QFuture<void> XShortcutsWidget::deleteOldStandardModel(QStandardItemModel **g_ppOldModel)
// {
// #if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
//     QFuture<void> future = QtConcurrent::run(&XShortcutsWidget::_deleteOldStandardModel, this, g_ppOldModel);
// #else
//     QFuture<void> future = QtConcurrent::run(this, &XShortcutsWidget::_deleteOldStandardModel, g_ppOldModel);
// #endif

//     return future;
// }
// #endif
void XShortcutsWidget::_deleteOldAbstractModel(QAbstractItemModel **g_ppOldModel)
{
    if (g_ppOldModel) {
        if (*g_ppOldModel) {
            delete (*g_ppOldModel);

            (*g_ppOldModel) = nullptr;
        }
    }
}

void XShortcutsWidget::_deleteOldStandardModel(QStandardItemModel **g_ppOldModel)
{
    if (g_ppOldModel) {
        if (*g_ppOldModel) {
            delete (*g_ppOldModel);

            (*g_ppOldModel) = nullptr;
        }
    }
}

void XShortcutsWidget::errorMessageSlot(const QString &sErrorMessage)
{
    QMessageBox::critical(XOptions::getMainWidget(this), tr("Error"), sErrorMessage);
}
