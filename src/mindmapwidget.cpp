#include "mindmapwidget.h"
#include "ui_mindmapwidget.h"

MindMapWidget::MindMapWidget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::MindMapWidget)
{
    ui->setupUi(this);
    m_selObject = nullptr;
    m_rootPoint = QPoint(32, 0);
    m_mousedownFlag = false;
    m_projectId = -1;
    m_cutObject = nullptr;
    m_backgroundId = 0;
    setMouseTracking(true);
}

MindMapWidget::~MindMapWidget()
{
    delete ui;
}

void MindMapWidget::openProject(int id)
{
    m_rootPoint = QPoint(32, 0);
    m_projectId = id;
    clear();
    addNode(-1, id);
    loadBackground();
    update();
    m_myDao->sqliteWrapper->execute(QString("insert into mind_history(pid) values (%1)").arg(id));
}

void MindMapWidget::paintEvent(QPaintEvent* event)
{
    drawImage(this, rect().width(), rect().height());
}

void MindMapWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
    emit onDoubleClick();
}

void MindMapWidget::drawRemark(QPainter& painter)
{
    QFont font;
    if (m_selObject != nullptr) {
        QString remark = m_selObject->remark().trimmed();
        if (remark != nullptr) {

            QStringList infos = remark.split("\n");
            int index = getMaxLen(infos);
            QRect rc;
            QFontMetrics fm = painter.fontMetrics();
            rc.setRect(m_selObject->rect.right() + 32,
                m_selObject->rect.top(),
                fm.width(infos[index]) + 64,
                fm.height() * infos.count());
            painter.setPen(QColor(197, 197, 197));
            painter.setBrush(QBrush(QColor(255, 255, 255, 200)));
            painter.drawRect(rc);
            painter.drawLine(QPoint(m_selObject->rect.right(),
                                 m_selObject->rect.top() + m_selObject->rect.height() / 2),
                QPoint(rc.left(), rc.top()));
            int l = rc.left() + 32;
            int t = rc.top();
            painter.setPen(Qt::black);
            font = painter.font();
            font.setPointSize(11);
            font.setBold(false);
            painter.setFont(font);
            for (int i = 0; i < infos.count(); i++) {
                QRect textRc;
                textRc.setRect(l, t, rc.width() - 32, fm.height());
                painter.drawText(textRc, Qt::AlignLeft | Qt::AlignVCenter, infos[i]);
                t += fm.height();
            }
        }
    }
}

void MindMapWidget::clear()
{
    qDeleteAll(m_mindMapObjects);
    m_mindMapObjects.clear();
}

void MindMapWidget::addNode(int pid, int id)
{
    int i = 1;
    if (pid == -1) {
        MindMapObject* obj = new MindMapObject(this);
        QSqlQuery qry;
        QString sql = QString("select pid,id,name,remark,link,fontcolor,backColor,sxh from mind_data where id=%1 order by sxh,id").arg(id);
        m_myDao->sqliteWrapper->select(sql, qry);
        qry.next();
        obj->setPid(qry.value(0).toInt());
        obj->setId(qry.value(1).toInt());
        obj->setName(qry.value(2).toString());
        obj->setRemark(qry.value(3).toString());
        obj->setLink(qry.value(4).toString());
        int fontcolor = qry.value(5).isNull() ? 0 : qry.value(5).toInt();
        obj->setFontColor(fontcolor);

        int backColor = qry.value(6).isNull() ? ColorTable::lastIndex() : qry.value(6).toInt();
        obj->setBackColor(backColor);
        int sxh = qry.value(7).toInt();
        if (sxh != i) {
            m_myDao->sqliteWrapper->execute(QString("update mind_data set sxh=%1 where id=%2").arg(i).arg(obj->id()));
        }
        obj->setSxh(i);
        addMarkNodes(obj);
        m_mindMapObjects << obj;
        addNode(obj->id());

    } else {

        QSqlQuery qry;
        QString sql = QString("select pid,id,name,remark,link,fontcolor,backColor,sxh from mind_data where pid=%1  order by sxh,id").arg(pid);
        m_myDao->sqliteWrapper->select(sql, qry);
        i = 1;
        while (qry.next()) {
            MindMapObject* obj = new MindMapObject(this);
            obj->setPid(qry.value(0).toInt());
            obj->setId(qry.value(1).toInt());
            obj->setName(qry.value(2).toString());
            obj->setRemark(qry.value(3).toString());
            obj->setLink(qry.value(4).toString());
            int fontcolor = qry.value(5).isNull() ? 0 : qry.value(5).toInt();
            int sxh = qry.value(7).toInt();
            if (sxh != i) {
                m_myDao->sqliteWrapper->execute(QString("update mind_data set sxh=%1 where id=%2").arg(i).arg(obj->id()));
            }
            obj->setSxh(i);
            obj->setFontColor(fontcolor);
            addMarkNodes(obj);
            m_mindMapObjects << obj;
            addNode(obj->id());
            i++;
        }
    }
}

MindMapObject* MindMapWidget::addChildNode(MindMapObject* pobj, int id, QString name)
{
    MindMapObject* obj = new MindMapObject(this);
    obj->setPid(pobj->id());
    obj->setId(id);
    obj->setName(name);
    m_mindMapObjects << obj;
    update();
    return obj;
}

void MindMapWidget::setMyDao(MyDao* myDao)
{
    m_myDao = myDao;
}

void MindMapWidget::drawNode(int pid, int px, int py, int x, int y, QPainter& painter)
{

    int m = 0;
    QFont font;
    QPoint pt = mapFromGlobal(cursor().pos());
    for (int i = 0; i < m_mindMapObjects.count(); i++) {
        MindMapObject* obj = m_mindMapObjects.at(i);
        if (obj->pid() == pid) {

            QRect rc;
            int n = 0;
            getNodeCount(obj, n);
            n = n == 0 ? 1 : n;
            n = n * 34;
            font = painter.font();
            font.setBold(false);
            font.setPointSize(14);
            painter.setFont(font);
            QFontMetrics fm = painter.fontMetrics();
            int reMarkWidth = obj->remark().trimmed() == "" ? 0 : 12;
            rc.setRect(x, y + n / 2 + m, fm.width(obj->name()) + 8 + reMarkWidth + 8 + obj->markNodes.count() * 16 + 64, 24);
            obj->remarkRect.setRect(x + fm.width(obj->name()) + 32, rc.top() + (rc.height() - 15) / 2, 12, 15);
            obj->addRect.setRect(x + fm.width(obj->name()) + 8 + reMarkWidth + obj->markNodes.count() * 16 + 64, rc.top() + rc.height() - 8, 16, 16);
            if (obj == m_selObject) {
                painter.setBrush(QColor(204, 232, 255));
            } else {
                if (obj->backColor() == ColorTable::lastIndex()) {
                    painter.setBrush(Qt::NoBrush);
                } else {
                    painter.setBrush(QBrush(ColorTable::getColor(obj->backColor())));
                }
            }

            QRect fillRc = rc;
            fillRc.setRight(rc.right() - 16);
            obj->rect = fillRc;
            painter.fillRect(fillRc, painter.brush());
            QPen pen;
            painter.setPen(Qt::black);
            font = painter.font();
            font.setBold(false);
            font.setPointSize(14);
            painter.setFont(font);
            QRect textRc = rc;
            textRc.setLeft(rc.left() + 16);
            pen = painter.pen();
            QColor fontColor = m_selObject == obj ? Qt::black : ColorTable::getColor(obj->fontColor());
            pen.setColor(fontColor);
            painter.setPen(pen);
            painter.drawText(textRc, obj->name(), Qt::AlignLeft | Qt::AlignVCenter);
            if (reMarkWidth != 0) {
                QPixmap remarkPix(":/res/remark.png");
                painter.drawPixmap(obj->remarkRect, remarkPix, remarkPix.rect());
            }
            int markNodeLeft = obj->remarkRect.right() + 8;
            for (int mid : obj->markNodes) {
                QRect mRc;
                mRc.setRect(markNodeLeft, rc.top() + (rc.height() - 12) / 2, 12, 12);
                QPixmap mPix(QString(":/res/flags/%1.png").arg(mid));
                painter.drawPixmap(mRc, mPix, mPix.rect());
                markNodeLeft += 16;
            }
            pen = painter.pen();
            pen.setWidth(3);
            MindMapObject* pobj = obj->parentObj();
            if (pobj == nullptr) {
                pen.setColor(obj->rootColor());

            } else {
                pen.setColor(pobj->lineColor());
            }
            painter.setPen(pen);
            painter.drawLine(rc.left(), rc.bottom() + 1, rc.right(), rc.bottom() + 1);

            m += n;
            if (pid != -1) {
                QPainterPath path;
                painter.setBrush(Qt::NoBrush);
                QPoint pts[4];
                pts[0] = QPoint(px, py);
                pts[3] = QPoint(rc.x(), rc.y() + rc.height());
                pts[1] = QPoint((pts[0].x() + pts[3].x()) / 2, pts[0].y());
                pts[2] = QPoint((pts[0].x() + pts[3].x()) / 2, pts[3].y());
                path.moveTo(pts[0]);
                path.cubicTo(pts[1], pts[2], pts[3]);
                painter.drawPath(path);
            }

            drawNode(obj->id(), rc.right(), rc.top() + rc.height(), rc.right() + 24, rc.top() - n / 2, painter);
        }
    }

    if (pid != -1) {
        QRect rc;
        rc.setRect(px - 8, py - 8, 16, 16);
        painter.setPen(Qt::NoPen);
        painter.setBrush(QBrush(QColor(10, 123, 251)));
        painter.drawEllipse(rc);
        painter.setPen(Qt::white);
        font = painter.font();
        font.setPointSize(24);
        font.setBold(true);
        painter.setFont(font);
        painter.drawText(rc, Qt::AlignCenter, "+");
    }
}

void MindMapWidget::mousePressEvent(QMouseEvent* event)
{
    m_mousedownFlag = true;
    QPoint pt = mapFromGlobal(cursor().pos());
    m_mousedownPoint = pt;
    for (int i = 0; i < m_mindMapObjects.count(); i++) {
        MindMapObject* obj = m_mindMapObjects.at(i);
        if (obj->addRect.contains(pt)) {
            m_selObject = obj;
            onAddChildNodeEvent();
            update();
            return;
        }
        if (obj->rect.contains(pt)) {
            m_selObject = obj;
            update();
#ifdef Q_OS_ANDROID
            QElapsedTimer t;
            t.start();
            while (t.elapsed() < 500) {
                QApplication::processEvents();
                if (m_mousedownFlag == false) {
                    return;
                }
            }
            showPopMenu();
#else
            if (event->button() == Qt::RightButton) {
                showPopMenu();
            }
#endif

            return;
        }
    }
    m_selObject = nullptr;
    update();
}

MindMapObject* MindMapWidget::selObject()
{
    return m_selObject;
}

int MindMapWidget::getNodeNeedCount(MindMapObject* pobj)
{
    int n = 1;
    int m = 0;
    for (int i = 0; i < m_mindMapObjects.count(); i++) {
        MindMapObject* obj = m_mindMapObjects.at(i);
        if (obj->pid() == pobj->id()) {
            m += getNodeNeedCount(obj);
        }
    }
    if (m > n) {
        return m;
    } else {
        return n;
    }
}

void MindMapWidget::getMindWidgetChildNum(MindMapObject* pobj)
{
    pobj->childNum = 0;
    for (int i = 0; i < m_mindMapObjects.count(); i++) {
        MindMapObject* obj = m_mindMapObjects.at(i);
        if (obj->pid() == pobj->id()) {
            obj->setParentObj(pobj);
            pobj->childNum++;
        }
    }
}

void MindMapWidget::getMindWidgetsChildNum()
{

    for (int i = 0; i < m_mindMapObjects.count(); i++) {
        MindMapObject* obj = m_mindMapObjects.at(i);
        getMindWidgetChildNum(obj);
    }
}

void MindMapWidget::getNodeCount(MindMapObject* pobj, int& n)
{
    for (int i = 0; i < m_mindMapObjects.count(); i++) {
        MindMapObject* obj = m_mindMapObjects.at(i);
        if (pobj->id() == obj->pid()) {
            if (obj->childNum == 0) {
                n++;
            }
            getNodeCount(obj, n);
        }
    }
}

void MindMapWidget::mouseReleaseEvent(QMouseEvent* event)
{
    m_mousedownFlag = false;
}

void MindMapWidget::getChilds(int pid, QList<int>& ids)
{
    for (int i = 0; i < m_mindMapObjects.count(); i++) {
        MindMapObject* obj = m_mindMapObjects.at(i);
        if (obj->pid() == pid) {
            qDebug() << obj->name();
            ids << obj->id();
            getChilds(obj->id(), ids);
        }
    }
}

void MindMapWidget::wheelEvent(QWheelEvent* event)
{
    m_rootPoint.setY(m_rootPoint.y() + event->delta());
    update();
}

void MindMapWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (m_mousedownFlag) {
        QPoint pt = mapFromGlobal(cursor().pos());
        int x = pt.x() - m_mousedownPoint.x();
        int y = pt.y() - m_mousedownPoint.y();
        m_rootPoint.setX(m_rootPoint.x() + x);
        m_rootPoint.setY(m_rootPoint.y() + y);
        m_mousedownPoint = pt;
    }
    update();
}

void MindMapWidget::editNode(MindMapObject* selObj, QString name)
{
    selObj->setName(name);
    update();
}

void MindMapWidget::delSelObject()
{

    if (m_selObject == nullptr) {
        return;
    }
    if (m_selObject->pid() == -1) {
        return;
    }
    int pid = m_selObject->pid();
    int ret = QMessageBox::question(this, "提示", "确定删除选中结点吗?");
    if (ret == QMessageBox::No) {
        return;
    }
    QList<int> ids;
    int id = m_selObject->id();
    m_selObject = nullptr;
    ids << id;
    getChilds(id, ids);
    for (int i = 0; i < ids.count(); i++) {
        deleteNodeById(ids.at(i));
    }
    reSetOrder(pid);
    update();
}

void MindMapWidget::deleteNodeById(int id)
{
    m_myDao->sqliteWrapper->execute(QString("delete from mind_data where id=%1").arg(id));
    for (int i = 0; i < m_mindMapObjects.count(); i++) {
        MindMapObject* obj = m_mindMapObjects.at(i);
        if (obj != nullptr) {
            if (obj->id() == id) {
                delete obj;
                m_mindMapObjects.removeAt(i);

                return;
            }
        }
    }
}

int MindMapWidget::projectId()
{
    return m_projectId;
}

void MindMapWidget::deleteProject()
{
    m_selObject = nullptr;
    QList<int> ids;
    ids << m_projectId;
    getChilds(m_projectId, ids);
    for (int i = 0; i < ids.count(); i++) {
        deleteNodeById(ids.at(i));
    }

    m_myDao->sqliteWrapper->execute(QString("delete mind_data where pid=%1").arg(m_projectId));
    m_projectId = -1;
    update();
}

void MindMapWidget::showPopMenu()
{
    QPoint pt = m_selObject->rect.bottomLeft();
    pt = mapToGlobal(pt);
    QMenu* menu = new QMenu(this);
    QStringList menuNames;
    menuNames << "打开链接"
              << "使用百度搜索\"" + m_selObject->name() + "\""
              << "复制文本"
              << "设置文字颜色"
              << "设置文字背景"
              << "应用上次文字颜色效果"
              << "应用上次文字背景效果"
              << "应用上次文字效果"
              << "标记结点"
              << "剪切结点"
              << "粘贴结点"
              << "上移结点"
              << "下移结点"
              << "删除结点";
    for (int i = 0; i < menuNames.count(); i++) {
        QAction* act = new QAction(this);
        act->setText(menuNames[i]);
        connect(act, &QAction::triggered, this, &MindMapWidget::onPopMenuTrigger);
        menu->addAction(act);
    }
    menu->exec(pt);
    delete menu;
    m_mousedownFlag = false;
}

void MindMapWidget::releaseMouseFlag()
{
    m_mousedownFlag = false;
    update();
}

void MindMapWidget::showEditMarkNodeDialog()
{
    if (m_selObject == nullptr) {
        return;
    }
    EditMarkNodeDialog* dlg = new EditMarkNodeDialog();
    dlg->setMarkNodes(m_selObject->markNodes);
    int ret = dlg->exec();
    if (ret == QDialog::Accepted) {
        m_myDao->sqliteWrapper->execute(QString("delete from mind_flag where pid=%1").arg(m_selObject->id()));
        m_selObject->markNodes = dlg->markNodes();
        for (int i : m_selObject->markNodes) {
            m_myDao->sqliteWrapper->execute(QString("insert into mind_flag (pid,flag) values (%1,%2)").arg(m_selObject->id()).arg(i));
        }
        update();
    }
    delete dlg;
}

void MindMapWidget::addMarkNodes(MindMapObject* obj)
{
    QString sql = QString("select flag from mind_flag where pid=%1 order by id").arg(obj->id());
    QSqlQuery qry;
    m_myDao->sqliteWrapper->select(sql, qry);
    while (qry.next()) {
        obj->markNodes << qry.value(0).toInt();
    }
}

void MindMapWidget::saveDataAsPng()
{
    QString path = QFileDialog::getSaveFileName(this, "导出为png", "导出.png", "png图片(*.png)");
    if (path == "") {
        return;
    }
    m_rootPoint = QPoint(32, 0);
    update();
    int w;
    int h;
    getDrawRect(w, h);
    QImage* img = new QImage(w * 2, h * 2, QImage::Format_RGB32);
    drawImage(img, w, h, 2);
    img->save(path, "png", 100);
    delete img;
}

void MindMapWidget::drawImage(QPaintDevice* paintDevice, int imgWidth, int imgHeight, int factor)
{
    getMindWidgetsChildNum();
    QPainter painter;
    painter.begin(paintDevice);
    painter.scale(factor, factor);
    painter.setRenderHints(QPainter::HighQualityAntialiasing | QPainter::TextAntialiasing);
    QRect rc;
    rc.setRect(0, 0, imgWidth + 1, imgHeight + 1);
    if (m_backgroundId == 0) {
        painter.fillRect(rc, Qt::white);
    } else {

        painter.drawPixmap(rc, m_backGroundPix, m_backGroundPix.rect());
    }

    int x = m_rootPoint.x();
    int y = m_rootPoint.y();
    drawNode(-1, x, y, x, y, painter);
    drawRemark(painter);
    painter.end();
}

void MindMapWidget::getDrawRect(int& w, int& h)
{
    w = 0;
    h = 0;
    for (int i = 0; i < m_mindMapObjects.count(); i++) {
        MindMapObject* obj = m_mindMapObjects.at(i);
        QRect rc = obj->rect;
        if (rc.right() > w) {
            w = rc.right();
        }
        if (rc.bottom() > h) {
            h = rc.bottom();
        }
    }
    w += 128;
    h += 128;
}

void MindMapWidget::reSetOrder(int pid)
{
    int n = 1;
    for (int i = 0; i < m_mindMapObjects.count(); i++) {
        MindMapObject* obj = m_mindMapObjects.at(i);
        if (obj->pid() == pid) {
            obj->setSxh(n);
            m_myDao->sqliteWrapper->execute(QString("update mind_data set sxh=%1 where id=%2").arg(n).arg(obj->id()));
            n++;
        }
    }
}

void MindMapWidget::setBackGroundId(int backGroundId)
{
    m_backgroundId = backGroundId;
    if (m_backgroundId == 0) {
        m_backGroundPix = QPixmap();
    } else {
        m_backGroundPix = QPixmap(QString(":/res/background/%1.jpg").arg(m_backgroundId));
    }
    update();
}

int MindMapWidget::backGroundId()
{
    return m_backgroundId;
}

void MindMapWidget::onAddChildNodeEvent()
{
    MindMapObject* selObj = selObject();
    if (selObj == nullptr) {
        return;
    }
    int pid = selObj->id();
    int maxSxh = getNewMaxSxh(pid);
    int id = m_myDao->addNode(selObj->id(), "", maxSxh);
    MindMapObject* obj = addChildNode(selObj, id, "");
    update();
    EditNodeDialog* dlg = new EditNodeDialog();
    dlg->setWindowTitle("添加结点");
    int ret = dlg->exec();
    if (ret == QDialog::Accepted) {
        QString name = dlg->getName();
        m_myDao->editNode(obj->id(), name);
        editNode(obj, name);

    } else {
        deleteNodeById(id);
        reSetOrder(pid);
    }
    delete dlg;
    releaseMouseFlag();
}

int MindMapWidget::getNewMaxSxh(int pid)
{
    QSqlQuery qry;
    QString sql;
    sql = QString("select max(sxh) from mind_data where pid=%1").arg(pid);
    m_myDao->sqliteWrapper->select(sql, qry);
    if (qry.next()) {
        return qry.value(0).toInt() + 1;
    } else {
        return 1;
    }
}

void MindMapWidget::showMindLineOut()
{
    QStringList infos;
    showMindLineOutInfo(-1, "", infos);
    ShowMindLineOutDialog* dlg = new ShowMindLineOutDialog();
    dlg->setInfos(infos);
    dlg->exec();
    delete dlg;
}

void MindMapWidget::showMindLineOutInfo(int pid, QString prefix, QStringList& infos)
{
    for (int i = 0; i < m_mindMapObjects.count(); i++) {
        MindMapObject* obj = m_mindMapObjects.at(i);
        if (obj->pid() == pid) {
            infos << prefix + "·" + obj->name();
            showMindLineOutInfo(obj->id(), prefix + "    ", infos);
        }
    }
}

void MindMapWidget::onPopMenuTrigger()
{
    m_mousedownFlag = false;
    QAction* act = static_cast<QAction*>(sender());
    if (act->text() == "打开链接") {
        if (m_selObject->link().startsWith("http")) {
            QDesktopServices::openUrl(m_selObject->link());
        } else {
            QDesktopServices::openUrl(QUrl::fromLocalFile(m_selObject->link()));
        }
    } else if (act->text().startsWith("使用百度搜索")) {
        QDesktopServices::openUrl("https://www.baidu.com/s?wd=" + m_selObject->name());
    } else if (act->text() == "剪切结点") {
        m_cutObject = m_selObject;
    } else if (act->text() == "粘贴结点") {
        if (m_cutObject == nullptr) {
            return;
        }
        int id = m_selObject->id();
        int newPid = m_selObject->id();
        m_cutObject->setPid(newPid);
        m_myDao->sqliteWrapper->execute(QString("update mind_data set pid=%1 where id=%2").arg(newPid).arg(id));
        m_cutObject = nullptr;
        update();
    } else if (act->text() == "标记结点") {
        showEditMarkNodeDialog();
    } else if (act->text() == "复制文本") {
        qApp->clipboard()->setText(m_selObject->name());
    } else if (act->text() == "设置文字颜色") {
        EditColorDialog* dlg = new EditColorDialog();
        int ret = dlg->exec();
        if (ret == QDialog::Accepted) {
            int color = dlg->colorIndex();
            m_selObject->setFontColor(color);
            ColorTable::fontColorIndex = color;
            m_myDao->sqliteWrapper->execute(QString("update mind_data set fontColor=%1 where id=%2")
                                                .arg(color)
                                                .arg(m_selObject->id()));
            update();
        }
        delete dlg;
    } else if (act->text() == "应用上次文字颜色效果") {
        if (m_selObject == nullptr) {
            return;
        }
        int color = ColorTable::fontColorIndex;
        m_selObject->setFontColor(color);
        m_myDao->sqliteWrapper->execute(QString("update mind_data set fontColor=%1 where id=%2")
                                            .arg(color)
                                            .arg(m_selObject->id()));
        update();
    } else if (act->text() == "设置文字背景") {
        EditColorDialog* dlg = new EditColorDialog();
        int ret = dlg->exec();
        if (ret == QDialog::Accepted) {
            int color = dlg->colorIndex();
            m_selObject->setBackColor(color);
            ColorTable::backColorIndex = color;
            m_myDao->sqliteWrapper->execute(QString("update mind_data set backColor=%1 where id=%2")
                                                .arg(color)
                                                .arg(m_selObject->id()));
            update();
        }
        delete dlg;
    } else if (act->text() == "应用上次文字背景效果") {

        int color = ColorTable::backColorIndex;
        m_selObject->setBackColor(color);
        m_myDao->sqliteWrapper->execute(QString("update mind_data set backColor=%1 where id=%2")
                                            .arg(color)
                                            .arg(m_selObject->id()));
        update();
    } else if (act->text() == "应用上次文字效果") {
        if (m_selObject == nullptr) {
            return;
        }
        int color;
        color = ColorTable::fontColorIndex;
        m_selObject->setFontColor(color);
        m_myDao->sqliteWrapper->execute(QString("update mind_data set fontColor=%1 where id=%2")
                                            .arg(color)
                                            .arg(m_selObject->id()));
        color = ColorTable::backColorIndex;
        m_selObject->setBackColor(color);
        m_myDao->sqliteWrapper->execute(QString("update mind_data set backColor=%1 where id=%2")
                                            .arg(color)
                                            .arg(m_selObject->id()));
        update();
    } else if (act->text() == "上移结点") {
        int pid = m_selObject->pid();
        int m = m_mindMapObjects.indexOf(m_selObject);
        for (int i = m - 1; i >= 0; i--) {
            MindMapObject* obj = m_mindMapObjects.at(i);
            if (obj->pid() == pid) {
                m_mindMapObjects.swap(i, m);
                reSetOrder(pid);
                update();
                return;
            }
        }
    } else if (act->text() == "下移结点") {
        int pid = m_selObject->pid();
        int m = m_mindMapObjects.indexOf(m_selObject);
        for (int i = m + 1; i < m_mindMapObjects.count(); i++) {
            MindMapObject* obj = m_mindMapObjects.at(i);
            if (obj->pid() == pid) {
                m_mindMapObjects.swap(i, m);
                reSetOrder(pid);
                update();
                return;
            }
        }
    } else if (act->text() == "删除结点") {
        delSelObject();
    }
}

int MindMapWidget::getMaxLen(QStringList& stl)
{
    int n = 0;
    int index = 0;
    for (int i = 0; i < stl.count(); i++) {
        int m = stl[i].length();
        if (m > n) {
            n = m;
            index = i;
        }
    }

    return index;
}

void MindMapWidget::loadBackground()
{
    QSqlQuery qry;
    m_myDao->sqliteWrapper->select(QString("select bid from mind_background where pid=%1").arg(m_projectId), qry);
    if (qry.next()) {
        m_backgroundId = qry.value(0).toInt();
    } else {
        m_backgroundId = 0;
    }
    setBackGroundId(m_backgroundId);
}
