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
    m_screenshotFlag = false;
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
        QString sql = QString("select pid,id,name,remark,link,fontcolor,backColor,sxh,showNum,bold,italics,overline,underline,strikeOut,img from mind_data where id=%1 order by sxh,id").arg(id);
        m_myDao->sqliteWrapper->select(sql, qry);
        qry.next();
        addNodeInfo(obj, 1, qry);

        addNode(obj->id());

    } else {

        QSqlQuery qry;
        QString sql = QString("select pid,id,name,remark,link,fontcolor,backColor,sxh,showNum,bold,italics,overline,underline,strikeOut,img from mind_data where pid=%1  order by sxh,id").arg(pid);
        m_myDao->sqliteWrapper->select(sql, qry);
        i = 1;
        while (qry.next()) {
            MindMapObject* obj = new MindMapObject(this);
            addNodeInfo(obj, i, qry);
            addNode(obj->id());
            i++;
        }
    }
}

void MindMapWidget::addNodeInfo(MindMapObject* obj, int i, QSqlQuery& qry)
{
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
    obj->setBold(getQryValueBool(qry, "bold"));
    obj->setItalics(getQryValueBool(qry, "italics"));
    obj->setOverline(getQryValueBool(qry, "overline"));
    obj->setUnderline(getQryValueBool(qry, "underline"));
    obj->setStrikeOut(getQryValueBool(qry, "strikeOut"));
    obj->setHasImg(qry.value("img").isNull() == false);
    if (!qry.value("img").isNull()) {
        QByteArray ba = qry.value("img").toByteArray();
        QBuffer buf(&ba);
        QImage img;
        img.load(&buf, "png");
        QPixmap pix = QPixmap::fromImage(img);
        obj->setImg(pix);
    }
    addMarkNodes(obj);
    bool showNum = ((qry.value("shownum").isNull()) || (qry.value("showNum").toInt() == 0)) ? false : true;
    obj->setShowNum(showNum);
    m_mindMapObjects << obj;
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
            setNodeNameFont(obj, painter, font);
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
            //绘制编号
            if ((obj->parentObj() != nullptr) && (obj->parentObj()->showNum())) {
                painter.setPen(Qt::NoPen);
                painter.setBrush(numColor(obj->sxh()));
                QRect numRc;
                numRc.setRect(fillRc.left() + 2, fillRc.top() + (fillRc.height() - 12) / 2, 12, 12);
                painter.drawEllipse(numRc);
                painter.setPen(Qt::white);
                font = painter.font();
                setFontAsDefault(font);
                font.setPointSize(9);
                painter.setFont(font);
                painter.drawText(numRc, Qt::AlignCenter, QString("%1").arg(obj->sxh()));
            }

            painter.setPen(Qt::black);

            setNodeNameFont(obj, painter, font);
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
            //绘制图片
            if (obj->hasImg()) {
                QPixmap pix = obj->img();
                QPixmap tmpPix = pix.scaled(rc.width() - 16, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                QRect pixRc;
                pixRc.setRect(textRc.left() - 16, textRc.bottom() + 16, tmpPix.width(), tmpPix.height());
                painter.drawPixmap(pixRc, tmpPix, tmpPix.rect());
                painter.setBrush(Qt::NoBrush);
                painter.setPen(QColor(197, 197, 197));
                painter.drawRect(pixRc);
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

            m += n + (obj->hasImg() ? 132 : 0);
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
        font.setItalic(false);
        font.setUnderline(false);
        font.setOverline(false);
        font.setStrikeOut(false);
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
            showSelPopMenu();
#else
            if (event->button() == Qt::RightButton) {
                showSelPopMenu();
            }
#endif

            return;
        }
    }
    m_selObject = nullptr;
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
    if (m_screenshotFlag == true) {
        m_startShot = pt;
    }
    update();
}

MindMapObject* MindMapWidget::selObject()
{
    return m_selObject;
}

void MindMapWidget::setSelObject(MindMapObject* obj)
{
    m_selObject = obj;
    update();
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
            if (obj->hasImg()) {
                n += 4;
            }
            getNodeCount(obj, n);
        }
    }
}

void MindMapWidget::mouseReleaseEvent(QMouseEvent* event)
{
    m_mousedownFlag = false;
    if (m_screenshotFlag == true) {
        QPoint pt = mapFromGlobal(cursor().pos());
        QRect rc;
        getSelectRect(rc, m_startShot, pt);
        QPixmap pix = grab(rc);
        qApp->clipboard()->setPixmap(pix);
        m_screenshotFlag = false;
    }
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
    if (!m_mousedownFlag == true) {
        if (event->modifiers() == Qt::ControlModifier) {
            m_rootPoint.setX(m_rootPoint.x() + event->delta());
        } else {
            m_rootPoint.setY(m_rootPoint.y() + event->delta());
        }
    }

    update();
}

void MindMapWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (m_mousedownFlag) {
        QPoint pt = mapFromGlobal(cursor().pos());
        if (m_screenshotFlag == true) {

        } else {

            int x = pt.x() - m_mousedownPoint.x();
            int y = pt.y() - m_mousedownPoint.y();
            m_rootPoint.setX(m_rootPoint.x() + x);
            m_rootPoint.setY(m_rootPoint.y() + y);
            m_mousedownPoint = pt;
        }
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

void MindMapWidget::showSelPopMenu()
{
    QPoint pt = m_selObject->rect.bottomLeft();
    pt = mapToGlobal(pt);
    QMenu* menu = new QMenu(this);
    QStringList menuNames;
    menuNames << "打开链接"
              << "使用百度搜索\"" + m_selObject->name() + "\""
              << "复制文本"
              << "粘贴文本"
              << "添加剪贴板图片"
              << "删除图片"
              << "添加空结点"
              << "添加剪贴板内容为结点"
              << "标记结点"
              << "剪切结点"
              << "粘贴结点"
              << "上移结点"
              << "下移结点"
              << "删除结点"
              << "设置下级结点编号";
    for (int i = 0; i < menuNames.count(); i++) {
        if (menuNames[i] == "打开链接") {
            if (m_selObject->link().trimmed() == "") {
                continue;
            }
        } else if (menuNames[i] == "粘贴结点") {
            if (m_cutObject == nullptr) {
                continue;
            }
        } else if (menuNames[i] == "粘贴文本") {
            if (qApp->clipboard()->text() == "") {
                continue;
            }
        } else if (menuNames[i] == "添加剪贴板内容为结点") {
            if (qApp->clipboard()->text() == "") {
                continue;
            }
        } else if (menuNames[i] == "删除图片") {
            if (m_selObject->hasImg() == false) {
                continue;
            }
        }
        QAction* act = new QAction(this);
        act->setText(menuNames[i]);
        connect(act, &QAction::triggered, this, &MindMapWidget::onPopMenuTrigger);
        menu->addAction(act);
    }
    menu->exec(pt);
    delete menu;
    m_mousedownFlag = false;
}

void MindMapWidget::showPopMenu()
{
    QPoint pt = mapFromGlobal(cursor().pos());
    pt = mapToGlobal(pt);
    QMenu* menu = new QMenu(this);
    QStringList menuNames;
    menuNames
        << "开始截图"
        << "查找和替换";
    for (int i = 0; i < menuNames.count(); i++) {
        if (menuNames[i] == "打开链接") {
            if (m_selObject->link().trimmed() == "") {
                continue;
            }
        } else if (menuNames[i] == "粘贴结点") {
            if (m_cutObject == nullptr) {
                continue;
            }
        } else if (menuNames[i] == "粘贴文本") {
            if (qApp->clipboard()->text() == "") {
                continue;
            }
        } else if (menuNames[i] == "添加剪贴板内容为结点") {
            if (qApp->clipboard()->text() == "") {
                continue;
            }
        }
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

void MindMapWidget::saveDataAsMarkdown()
{
    QString path = QFileDialog::getSaveFileName(this, "导出为Markdown", "导出.md", "Markdown文件(*.md)");
    if (path == "") {
        return;
    }
    QStringList infos;
    showMindLineOutInfo(-1, "", infos, "#");
    QFile file(path);
    file.open(QIODevice::WriteOnly);
    QTextStream stm(&file);
    for (int i = 0; i < infos.count(); i++) {
        stm << infos[i] + "\n";
    }
    file.flush();
    file.close();
}

void MindMapWidget::saveDataAsTxtFile()
{
    QString path = QFileDialog::getSaveFileName(this, "导出为txt文件", "导出.txt", "txt文件(*.txt)");
    if (path == "") {
        return;
    }
    QStringList infos;
    showMindLineOutInfo(-1, "", infos, "");
    QFile file(path);
    file.open(QIODevice::WriteOnly);
    QTextStream stm(&file);
    for (int i = 0; i < infos.count(); i++) {
        stm << infos[i] + "\n";
    }
    file.flush();
    file.close();
}

void MindMapWidget::drawImage(QPaintDevice* paintDevice, int imgWidth, int imgHeight, int factor)
{
    getMindWidgetsChildNum();
    QPoint pt = mapFromGlobal(cursor().pos());
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
    if ((m_screenshotFlag == true) && (m_mousedownFlag == true)) {

        QRect drawRc, rc;
        getSelectRect(drawRc, m_startShot, pt);

        QPen pen = painter.pen();
        pen.setColor(Qt::black);
        pen.setStyle(Qt::DotLine);
        painter.setPen(pen);
        QColor color = QColor(0, 0, 0, 128);
        rc.setRect(0, 0, width(), drawRc.top());
        painter.fillRect(rc, QBrush(color));
        rc.setRect(0, drawRc.bottom() + 1, width(), height() - drawRc.bottom() - 1);
        painter.fillRect(rc, QBrush(color));

        rc.setRect(0, drawRc.top(), drawRc.left(), drawRc.height());
        painter.fillRect(rc, QBrush(color));
        rc.setRect(drawRc.right() + 1, drawRc.top(), width() - drawRc.right() - 1, drawRc.height());
        painter.fillRect(rc, QBrush(color));

        painter.setBrush(Qt::NoBrush);
        painter.drawRect(drawRc);
        pen = painter.pen();

        pen.setStyle(Qt::SolidLine);
        painter.setPen(pen);
    }
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
    obj->setSxh(maxSxh);
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

void MindMapWidget::onAddChildNodeEvent(QString name)
{
    MindMapObject* selObj = selObject();
    if (selObj == nullptr) {
        return;
    }
    int pid = selObj->id();
    int maxSxh = getNewMaxSxh(pid);
    int id = m_myDao->addNode(selObj->id(), name, maxSxh);
    addChildNode(selObj, id, name);
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

void MindMapWidget::showMindLineOutInfo(int pid, QString prefix, QStringList& infos, QString sign, QString space)
{
    for (int i = 0; i < m_mindMapObjects.count(); i++) {
        MindMapObject* obj = m_mindMapObjects.at(i);
        if (obj->pid() == pid) {
            if (sign == "#") {
                infos << prefix + sign + " " + space + obj->name();
                infos << "\n";
                showMindLineOutInfo(obj->id(), prefix + "#", infos, sign, space + "&emsp;&emsp;");
            } else {
                infos << prefix + sign + obj->name();
                showMindLineOutInfo(obj->id(), prefix + "    ", infos, sign);
            }
        }
    }
}

void MindMapWidget::getSelectRect(QRect& rc, QPoint pt1, QPoint pt2)
{
    int x = pt1.x();
    if (x > pt2.x()) {
        x = pt2.x();
    }
    int y = pt1.y();
    if (y > pt2.y()) {
        y = pt2.y();
    }
    rc.setRect(x, y, abs(pt1.x() - pt2.x()), abs(pt1.y() - pt2.y()));
}

QColor MindMapWidget::numColor(int sxh)
{
    QColor colors[7] = { QColor(0, 0, 0), QColor(230, 66, 72),
        QColor(249, 182, 42),
        QColor(85, 101, 251),
        QColor(187, 58, 172),
        QColor(43, 204, 91),
        QColor(28, 171, 252) };
    if ((sxh >= 1) && (sxh <= 5)) {
        return colors[sxh];
    } else {
        return colors[6];
    }
}

void MindMapWidget::setNodeFontStyle(int setType)
{
    if (m_selObject == nullptr) {
        return;
    }

    QStringList fieldNames;
    fieldNames << "bold"
               << "italics"
               << "overline"
               << "underline"
               << "strikeOut"
               << "backColor";
    if (setType <= 4) {
        bool b = m_selObject->value(setType);
        m_selObject->setValue(setType, !b);
        b = m_selObject->value(setType);
        m_myDao->sqliteWrapper->execute(QString("update mind_data set %1=%2 "
                                                "where id=%3")

                                            .arg(fieldNames[setType])
                                            .arg(b2i(b))
                                            .arg(m_selObject->id()));
    } else if (setType == SET_FONT_BACKCOLOR) {
        m_selObject->setBackColor(ColorTable::backColorIndex);
        m_myDao->sqliteWrapper->execute(QString("update mind_data set backcolor=%1 "
                                                "where id=%2")
                                            .arg(ColorTable::backColorIndex)
                                            .arg(m_selObject->id()));
    } else if (setType == SET_FONT_COLOR) {
        m_selObject->setFontColor(ColorTable::fontColorIndex);
        m_myDao->sqliteWrapper->execute(QString("update mind_data set fontcolor=%1 "
                                                "where id=%2")
                                            .arg(ColorTable::fontColorIndex)
                                            .arg(m_selObject->id()));
    }

    update();
}

void MindMapWidget::setFontStyle()
{
    if (m_selObject == nullptr) {
        return;
    }
    ColorTable::fontColorIndex = m_selObject->fontColor();
    ColorTable::backColorIndex = m_selObject->backColor();
    m_myDao->saveGlobalSet("fontcolorindex", ColorTable::fontColorIndex);
    m_myDao->saveGlobalSet("backcolorindex", ColorTable::backColorIndex);
}

void MindMapWidget::applyFontStyle()
{
    if (m_selObject == nullptr) {
        return;
    }
    m_selObject->setFontColor(ColorTable::fontColorIndex);
    m_selObject->setBackColor(ColorTable::backColorIndex);
    m_myDao->sqliteWrapper->execute(QString("update mind_data set fontColor=%1 where id=%2")
                                        .arg(ColorTable::fontColorIndex)
                                        .arg(m_selObject->id()));
    m_myDao->sqliteWrapper->execute(QString("update mind_data set backcolor=%1 where id=%2")
                                        .arg(ColorTable::backColorIndex)
                                        .arg(m_selObject->id()));
}

int MindMapWidget::b2i(bool b)
{
    return b == true ? 1 : 0;
}

void MindMapWidget::setNodeNameFont(MindMapObject* obj, QPainter& painter, QFont& font)
{
    font = painter.font();
    font.setBold(obj->bold());
    font.setItalic(obj->italics());
    font.setUnderline(obj->underline());
    font.setOverline(obj->overline());
    font.setStrikeOut(obj->strikOut());
    font.setPointSize(14);
    painter.setFont(font);
}

void MindMapWidget::setFontAsDefault(QFont& font)
{

    font.setItalic(false);
    font.setUnderline(false);
    font.setOverline(false);
    font.setStrikeOut(false);
    font.setBold(false);
}

void MindMapWidget::showBackColorEditDialog()
{
    EditColorDialog* dlg = new EditColorDialog();
    connect(dlg, &EditColorDialog::onAddColorIndex, this, &MindMapWidget::onAddColorIndex);
    int ret = dlg->exec();
    if (ret == QDialog::Accepted) {
        int color = dlg->colorIndex();
        ColorTable::backColorIndex = color;
        m_myDao->saveGlobalSet("backcolorindex", color);
        if (m_selObject != nullptr) {
            m_selObject->setBackColor(color);

            m_myDao->sqliteWrapper->execute(QString("update mind_data set backColor=%1 where id=%2")
                                                .arg(color)
                                                .arg(m_selObject->id()));
        }

        update();
    }
    delete dlg;
}

void MindMapWidget::showFontColorEditDialog()
{
    EditColorDialog* dlg = new EditColorDialog();
    connect(dlg, &EditColorDialog::onAddColorIndex, this, &MindMapWidget::onAddColorIndex);
    int ret = dlg->exec();
    if (ret == QDialog::Accepted) {
        int color = dlg->colorIndex();
        m_myDao->saveGlobalSet("fontcolorindex", color);
        ColorTable::fontColorIndex = color;
        if (m_selObject != nullptr) {
            m_selObject->setFontColor(color);

            m_myDao->sqliteWrapper->execute(QString("update mind_data set fontColor=%1 where id=%2")
                                                .arg(color)
                                                .arg(m_selObject->id()));
        }

        update();
    }
    delete dlg;
}

void MindMapWidget::saveMindMapAsNewProject()
{
    QString sql;
    int mpid;
    for (int i = 0; i < m_mindMapObjects.count(); i++) {
        MindMapObject* oldObj = m_mindMapObjects.at(i);
        sql = QString("insert into mind_data(name,remark,fontcolor,backcolor,sxh,shownum,bold,italics,"
                      "underline,overline,strikeOut) select name,remark,fontcolor,backcolor,sxh,shownum,bold,italics,"
                      "underline,overline,strikeOut from mind_data where id=%1")
                  .arg(oldObj->id());
        m_myDao->sqliteWrapper->execute(sql);
        int maxId = m_myDao->sqliteWrapper->getMaxId("mind_data");
        MindMapObject* newObj = oldObj->newObj();
        newObj->setId(maxId);
        if (oldObj->pid() == -1) {
            mpid = maxId;
            newObj->setPid(-1);
            m_myDao->sqliteWrapper->execute(QString("update mind_data set pid=-1 where id=%1").arg(maxId));
            QDateTime d = QDateTime::currentDateTime();
            QString s = oldObj->name() + d.toString("副本yyyyMMddhhmmss");
            sql = QString("update mind_data set name='%1' where id=%2")
                      .arg(s)
                      .arg(maxId);
            qDebug() << sql;
            m_myDao->sqliteWrapper->execute(sql);
        }
    }
    for (int i = 0; i < m_mindMapObjects.count(); i++) {
        MindMapObject* oldObj = m_mindMapObjects.at(i);
        MindMapObject* newObj = oldObj->newObj();
        if (newObj->pid() != -1) {
            int pid = newObj->srcObj()->parentObj()->newObj()->id();
            sql = QString("update mind_data set pid=%1 where id=%2").arg(pid).arg(newObj->id());
            m_myDao->sqliteWrapper->execute(sql);
            newObj->setPid(pid);
        }
    }
    for (int i = 0; i < m_mindMapObjects.count(); i++) {
        MindMapObject* oldObj = m_mindMapObjects.at(i);
        MindMapObject* newObj = oldObj->newObj();
        sql = QString("insert into mind_flag(pid,flag) "
                      "select %1 as pid,flag from mind_flag where pid=%2")
                  .arg(newObj->id())
                  .arg(oldObj->id());
        m_myDao->sqliteWrapper->execute(sql);
    }
    sql = QString("insert into mind_set(pid,bid) values (%1,%2)").arg(mpid).arg(m_backgroundId);
    m_myDao->sqliteWrapper->execute(sql);
    openProject(mpid);
}

QList<MindMapObject*>& MindMapWidget::mindMapObjects()
{
    return m_mindMapObjects;
}

MyDao* MindMapWidget::myDao()
{
    return m_myDao;
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
    } else if (act->text() == "粘贴文本") {
        if (m_selObject == nullptr) {
            return;
        }
        QString text = qApp->clipboard()->text();
        if (text.trimmed() == "") {
            return;
        }
        m_selObject->setName(text);
        m_myDao->editNode(m_selObject->id(), text);
        update();
    } else if (act->text() == "添加剪贴板图片") {
        if (m_selObject == nullptr) {
            return;
        }
        QPixmap pix = qApp->clipboard()->pixmap();
        if (!pix.isNull()) {
            m_selObject->setImg(pix);
            m_selObject->setHasImg(true);
            m_myDao->addPix(m_selObject->id(), pix);
            update();
        }
    } else if (act->text() == "删除图片") {
        if (m_selObject == nullptr) {
            return;
        }
        m_selObject->setHasImg(false);
        QPixmap pix = QPixmap();
        m_selObject->setImg(pix);
        m_myDao->sqliteWrapper->execute(QString("update mind_data set img=null where id=%1").arg(m_selObject->id()));
        update();
    } else if (act->text() == "添加空结点") {
        onAddChildNodeEvent("");
    } else if (act->text() == "添加剪贴板内容为结点") {
        onAddChildNodeEvent(qApp->clipboard()->text());
    } else if (act->text() == "开始截图") {
        m_screenshotFlag = true;
    } else if (act->text() == "设置下级结点编号") {
        m_selObject->setShowNum(!m_selObject->showNum());
        int shownum = m_selObject->showNum() == true ? 1 : 0;
        m_myDao->sqliteWrapper->execute(QString("update mind_data set shownum=%1 where id=%2").arg(shownum).arg(m_selObject->id()));
        update();
    } else if (act->text() == "查找和替换") {
        SearchAndReplaceDialog* dlg = new SearchAndReplaceDialog();
        dlg->setMindMapWidget(this);
        dlg->exec();
    }
}

void MindMapWidget::onAddColorIndex(int colorIndex)
{
    QString sql = QString("insert into mind_color(colorindex) values (%1)").arg(colorIndex);
    m_myDao->sqliteWrapper->execute(sql);
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
    m_myDao->sqliteWrapper->select(QString("select bid from mind_set where pid=%1").arg(m_projectId), qry);
    if (qry.next()) {
        m_backgroundId = qry.value(0).toInt();
    } else {
        m_backgroundId = 0;
    }
    setBackGroundId(m_backgroundId);
}

int MindMapWidget::getQryValue(QSqlQuery& qry, int index, int defaultValue)
{
    if (qry.value(index).isNull()) {
        return defaultValue;
    } else {
        return qry.value(index).toInt();
    }
}

int MindMapWidget::getQryValue(QSqlQuery& qry, QString fieldName, int defaultValue)
{
    if (qry.value(fieldName).isNull()) {
        return defaultValue;
    } else {
        return qry.value(fieldName).toInt();
    }
}

bool MindMapWidget::getQryValueBool(QSqlQuery& qry, QString fieldName, bool defaultValue)
{
    if (qry.value(fieldName).isNull()) {
        return defaultValue;
    } else {
        return qry.value(fieldName).toInt() == 1;
    }
}
