/* Copyright (c) 2021-2025 hors<horsicq@gmail.com>
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
#ifndef XSHORTCUTSWIDGET_H
#define XSHORTCUTSWIDGET_H

#include <QShortcut>
#include <QWidget>

#include "xoptions.h"
#include "xshortcuts.h"
#include <QAbstractItemModel>
#include <QStandardItemModel>

class XShortcutsWidget : public QWidget {
    Q_OBJECT

public:
    explicit XShortcutsWidget(QWidget *pParent = nullptr);
    ~XShortcutsWidget();

    virtual void setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions);
    XShortcuts *getShortcuts();
    XOptions *getGlobalOptions();
    void setActive(bool bState);
    void saveTableModel(QAbstractItemModel *pModel, const QString &sFileName);
    void saveTreeModel(QAbstractItemModel *pModel, const QString &sFileName);
    void saveTextEdit(QTextEdit *pTextEdit, const QString &sFileName);
    virtual void adjustView() = 0;
    void adjustViewChildren(QWidget *pWidget);
    void setGlobalChildren(QWidget *pWidget, XShortcuts *pShortcuts, XOptions *pXOptions);
    QString getOpenFileName(const QString &sDirectory);
    void _blockSignals(QObject **ppObjects, qint32 nCount, bool bState);
    virtual void reloadShortcuts();
    void setWidgetFocus();
    void deleteOldAbstractModel(QAbstractItemModel **g_ppOldModel);
    // QFuture<void> deleteOldStandardModel(QStandardItemModel **g_ppOldModel);
    virtual void setReadonly(bool bState);
    virtual bool isReadonly();
    virtual void reloadData(bool bSaveSelection);
    virtual void setLocation(quint64 nLocation, qint32 nLocationType, qint64 nSize);

private:
    void _deleteOldAbstractModel(QAbstractItemModel **g_ppOldModel);
    void _deleteOldStandardModel(QStandardItemModel **g_ppOldModel);

signals:
    void dataChanged(qint64 nDeviceOffset, qint64 nDeviceSize);
    void currentLocationChanged(quint64 nLocation, qint32 nLocationType, qint64 nSize);
    void followLocation(quint64 nLocation, qint32 nLocationType, qint64 nSize, qint32 nWidgetType);

protected slots:
    void errorMessageSlot(const QString &sErrorMessage);

protected:
    bool eventFilter(QObject *pObj, QEvent *pEvent) override;
    virtual void registerShortcuts(bool bState) = 0;

private:
    bool g_bIsActive;
    XShortcuts *g_pShortcuts;
    XShortcuts g_scEmpty;
    XOptions *g_pXOptions;
    XOptions g_xOptionsEmpty;
    bool g_bIsReadonly;
};

#endif  // XSHORTCUTSWIDGET_H
