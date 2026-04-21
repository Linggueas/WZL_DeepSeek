#ifndef FILEDIALOG_H
#define FILEDIALOG_H

#include <QWidget>
#include <QMouseEvent>

namespace Ui {
class FileDialog;
}

class FileDialog : public QWidget
{
    Q_OBJECT

public:
    explicit FileDialog(QWidget *parent = nullptr,const QString &file_name = "");
    ~FileDialog();
    QString getFileName() const;

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    Ui::FileDialog *ui;
    QString m_fileName;
};

#endif // FILEDIALOG_H
