// settingswindow.cpp
#include "settingswindow.h"
#include "settingsitem.h"
#include "comboboxfactory.h"
#include "spinboxfactory.h"
#include "checkboxfactory.h"
#include "lineeditfactory.h"
#include "pushbuttonfactory.h"
#include "filebrowsefactory.h"
#include "colordialogfactory.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTreeWidget>
#include <QStackedWidget>
#include <QScrollArea>
#include <QFrame>
#include <QLabel>
#include <QTreeWidgetItem>
#include <QCloseEvent>
#include <QLineEdit>
#include <QHeaderView>
#include <QDebug>
#include <QSettings>
#include <QRegularExpression>
#include <QApplication>

SettingsWindow::SettingsWindow(QWidget* parent) : QWidget(parent) {
    setupUI();
    createSettingsTree();
    setupConnections();
    collectAllSettingsItems();

    loadSettings();
    connectSignalsForAutoSave();

    searchTimer = new QTimer(this);
    searchTimer->setSingleShot(true);
    searchTimer->setInterval(300);
    connect(searchTimer, &QTimer::timeout, this, &SettingsWindow::performSearch);
}

SettingsWindow::~SettingsWindow() {
    delete rootItem;
    rootItem = nullptr;
}

void SettingsWindow::collectAllSettingsItems() {
    allSettingsItems.clear();
    itemToTreeItem.clear();

    QList<SettingsItem*> allItems = rootItem->getAllChildren();
    allItems.prepend(rootItem);

    for (SettingsItem* item : allItems) {
        allSettingsItems.append(item);
    }


    QTreeWidgetItemIterator it(treeWidget);
    while (*it) {
        SettingsItem* item = (*it)->data(0, Qt::UserRole).value<SettingsItem*>();
        if (item) {
            itemToTreeItem[item] = *it;
        }
        ++it;
    }
}

void SettingsWindow::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    setupSearchUI();

    QHBoxLayout* contentLayout = new QHBoxLayout();
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setSpacing(0);

    treeWidget = new QTreeWidget(this);
    treeWidget->setFixedWidth(280);
    treeWidget->setHeaderHidden(true);
    treeWidget->setAnimated(true);
    treeWidget->setIndentation(20);

    stackedWidget = new QStackedWidget(this);

    contentLayout->addWidget(treeWidget);
    contentLayout->addWidget(stackedWidget, 1);

    mainLayout->addLayout(contentLayout);

    setWindowTitle("Settings");
    setMinimumSize(1000, 700);
    resize(1200, 800);
}

void SettingsWindow::setupSearchUI() {
    QWidget* searchWidget = new QWidget(this);

    QVBoxLayout* searchLayout = new QVBoxLayout(searchWidget);
    searchLayout->setContentsMargins(15, 12, 15, 12);
    searchLayout->setSpacing(8);

    QHBoxLayout* searchInputLayout = new QHBoxLayout();

    searchLineEdit = new QLineEdit(searchWidget);
    searchLineEdit->setPlaceholderText("Search settings... (type to filter)");
    searchLineEdit->setClearButtonEnabled(true);

    searchInputLayout->addWidget(searchLineEdit, 1);

    searchResultsLabel = new QLabel(searchWidget);
    searchResultsLabel->setVisible(false);

    searchLayout->addLayout(searchInputLayout);
    searchLayout->addWidget(searchResultsLabel);


    QVBoxLayout* mainLayout = qobject_cast<QVBoxLayout*>(this->layout());
    if (mainLayout) {
        mainLayout->insertWidget(0, searchWidget);
    }


    connect(searchLineEdit, &QLineEdit::textChanged,
            this, &SettingsWindow::onSearchTextChanged);
}

void SettingsWindow::createSettingsTree() {
    rootItem = new SettingsItem(nullptr, "Settings", "root", "Application Settings");

    SettingsItem* mainGroup = new SettingsItem(rootItem, "Main Settings", "main_group", "General application settings");
    SettingsItem* templateGroup = new SettingsItem(rootItem, "Template Settings", "template_group", "File template settings");
    SettingsItem* colorGroup = new SettingsItem(rootItem, "Appearance", "appearance_group", "Visual appearance settings");


    new SettingsItem(mainGroup, "Language", "1", "Select interface language",
                     new ComboBoxFactory("English", QStringList() << "English" << "Russian" << "Spanish"), true);

    new SettingsItem(mainGroup, "Autostart", "2", "Run application on system startup",
                     new CheckBoxFactory(true), true);

    new SettingsItem(mainGroup, "Timeout", "3", "Request timeout in milliseconds",
                     new SpinBoxFactory(300, 100, 10000), true);

    new SettingsItem(templateGroup, "File Template", "4", "Template for file searching",
                     new LineEditFactory("*.png"), true);

    new SettingsItem(templateGroup, "Storage Path", "5", "Location where files will be stored",
                     new FileBrowseFactory(new LineEditFactory("D:/storage"), new PushButtonFactory("Browse...")), true);

    new SettingsItem(colorGroup, "Theme Color", "6", "Choose application theme color",
                     new ColorDialogFactory(new LineEditFactory("#0078d4"), new PushButtonFactory("Choose Color")), true);

    new SettingsItem(colorGroup, "Font Size", "7", "Application font size",
                     new SpinBoxFactory(12, 8, 24), true);

    buildTreeWidget();


    createPagesForGroups();

    if (treeWidget->topLevelItemCount() > 0) {
        treeWidget->setCurrentItem(treeWidget->topLevelItem(0));
    }
}

void SettingsWindow::buildTreeWidget() {
    treeWidget->clear();

    for (int i = 0; i < rootItem->childCount(); ++i) {
        addItemToTreeWidget(rootItem->child(i), nullptr);
    }

    treeWidget->expandAll();
}

void SettingsWindow::addItemToTreeWidget(SettingsItem* settingsItem, QTreeWidgetItem* parentTreeItem) {
    QTreeWidgetItem* treeItem;
    if (parentTreeItem) {
        treeItem = new QTreeWidgetItem(parentTreeItem);
    } else {
        treeItem = new QTreeWidgetItem(treeWidget);
    }

    treeItem->setText(0, settingsItem->name());
    treeItem->setData(0, Qt::UserRole, QVariant::fromValue(settingsItem));

    if (settingsItem->isGroup()) {
        QFont font = treeItem->font(0);
        font.setBold(true);
        treeItem->setFont(0, font);
    }

    for (int i = 0; i < settingsItem->childCount(); ++i) {
        addItemToTreeWidget(settingsItem->child(i), treeItem);
    }
}

void SettingsWindow::createPagesForGroups() {
    QList<SettingsItem*> allItems = rootItem->getAllChildren();
    allItems.prepend(rootItem);

    for (SettingsItem* item : allItems) {
        if (item->isGroup()) {
            createPageForGroup(item);
        }
    }
}

void SettingsWindow::createPageForGroup(SettingsItem* groupItem) {
    QScrollArea* scrollArea = new QScrollArea(stackedWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);

    QWidget* contentWidget = new QWidget(scrollArea);
    QVBoxLayout* contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setSpacing(20);
    contentLayout->setContentsMargins(30, 30, 30, 30);

    QLabel* titleLabel = new QLabel(groupItem->name(), contentWidget);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(20);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    contentLayout->addWidget(titleLabel);

    if (!groupItem->description().isEmpty()) {
        QLabel* descLabel = new QLabel(groupItem->description(), contentWidget);
        descLabel->setWordWrap(true);
        contentLayout->addWidget(descLabel);
    }

    QFrame* separator = new QFrame(contentWidget);
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    contentLayout->addWidget(separator);

    bool hasSettings = false;
    for (int i = 0; i < groupItem->childCount(); ++i) {
        SettingsItem* child = groupItem->child(i);
        if (!child->isGroup()) {
            QHBoxLayout* itemLayout = child->createWidget();
            if (itemLayout) {
                if (hasSettings) {
                    QFrame* itemSeparator = new QFrame(contentWidget);
                    itemSeparator->setFrameShape(QFrame::HLine);
                    itemSeparator->setFrameShadow(QFrame::Sunken);
                    contentLayout->addWidget(itemSeparator);
                }
                contentLayout->addLayout(itemLayout);
                hasSettings = true;
            }
        }
    }

    if (!hasSettings) {
        QLabel* emptyLabel = new QLabel("No settings available", contentWidget);
        emptyLabel->setAlignment(Qt::AlignCenter);
        contentLayout->addWidget(emptyLabel);
    }

    contentLayout->addStretch();
    scrollArea->setWidget(contentWidget);

    groupPages[groupItem] = scrollArea;
    stackedWidget->addWidget(scrollArea);
}

void SettingsWindow::setupConnections() {
    connect(treeWidget, &QTreeWidget::currentItemChanged,
            this, &SettingsWindow::onTreeItemChanged);
}

void SettingsWindow::onTreeItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous) {
    Q_UNUSED(previous);

    if (!current) return;

    SettingsItem* settingsItem = current->data(0, Qt::UserRole).value<SettingsItem*>();
    if (settingsItem && settingsItem->isGroup()) {
        if (groupPages.contains(settingsItem)) {
            stackedWidget->setCurrentWidget(groupPages[settingsItem]);
        }
    }
}

void SettingsWindow::onSearchTextChanged(const QString& text) {
    Q_UNUSED(text);
    searchTimer->start();
}

void SettingsWindow::performSearch() {
    QString searchText = searchLineEdit->text().trimmed();

    if (searchText.isEmpty()) {
        showAllItems();
        searchResultsLabel->setVisible(false);
    } else {
        filterSettings(searchText);
        searchResultsLabel->setVisible(true);
    }
}

void SettingsWindow::filterSettings(const QString& searchText) {
    QString searchTextLower = searchText.toLower();
    int matchCount = 0;

    if (originalExpansionState.isEmpty()) {
        QTreeWidgetItemIterator it(treeWidget);
        while (*it) {
            originalExpansionState[*it] = (*it)->isExpanded();
            ++it;
        }
    }

    QSet<SettingsItem*> matchingItems;
    QSet<SettingsItem*> parentItems;

    for (SettingsItem* item : allSettingsItems) {
        if (itemMatchesSearch(item, searchTextLower)) {
            matchingItems.insert(item);
            matchCount++;

            SettingsItem* parent = item->parent();
            while (parent) {
                parentItems.insert(parent);
                parent = parent->parent();
            }
        }
    }

    QTreeWidgetItemIterator it(treeWidget);
    while (*it) {
        SettingsItem* item = (*it)->data(0, Qt::UserRole).value<SettingsItem*>();
        bool isVisible = matchingItems.contains(item) || parentItems.contains(item);

        (*it)->setHidden(!isVisible);

        if (isVisible) {
            if (item && item->isGroup()) {
                (*it)->setExpanded(true);
            }

            if (matchingItems.contains(item) && item && !item->isGroup()) {
                (*it)->setBackground(0, QColor("#FFEB3B"));
            } else {
                (*it)->setBackground(0, QColor(Qt::transparent));
            }
        }

        ++it;
    }

    for (auto it = groupPages.begin(); it != groupPages.end(); ++it) {
        updatePageVisibility(it.key(), searchTextLower);
    }

    if (matchCount > 0) {
        searchResultsLabel->setText(QString("Found %1 setting(s)").arg(matchCount));
    } else {
        searchResultsLabel->setText("No settings found");
    }
}

bool SettingsWindow::itemMatchesSearch(SettingsItem* item, const QString& searchText) const {
    if (!item) return false;

    if (item->name().toLower().contains(searchText)) {
        return true;
    }

    if (item->description().toLower().contains(searchText)) {
        return true;
    }

    if (item->id().toLower().contains(searchText)) {
        return true;
    }

    if (item->isGroup()) {
        for (int i = 0; i < item->childCount(); ++i) {
            if (itemMatchesSearch(item->child(i), searchText)) {
                return true;
            }
        }
    }

    return false;
}

void SettingsWindow::updatePageVisibility(SettingsItem* groupItem, const QString& searchText) {
    if (!groupPages.contains(groupItem)) return;

    QScrollArea* scrollArea = groupPages[groupItem];
    QWidget* contentWidget = scrollArea->widget();
    if (!contentWidget) return;

    bool groupMatches = itemMatchesSearch(groupItem, searchText);
    bool hasMatchingChildren = false;

    for (int i = 0; i < groupItem->childCount(); ++i) {
        if (itemMatchesSearch(groupItem->child(i), searchText)) {
            hasMatchingChildren = true;
            break;
        }
    }

    QLayout* layout = contentWidget->layout();
    if (!layout) return;

    for (int i = 0; i < layout->count(); ++i) {
        QLayoutItem* item = layout->itemAt(i);
        if (!item) continue;

        if (QWidget* widget = item->widget()) {
            bool isTitle = (i == 0);
            widget->setVisible(isTitle || hasMatchingChildren || groupMatches);
        }
    }

    QList<QLabel*> labels = contentWidget->findChildren<QLabel*>();
    for (QLabel* label : labels) {
        QString text = label->text();
        if (text.contains("<span")) {
            text = text.remove(QRegularExpression("<[^>]*>"));
            label->setText(text);
        }

        if (!searchText.isEmpty()) {
            highlightTextInLabel(label, searchText);
        }
    }
}

void SettingsWindow::highlightTextInLabel(QLabel* label, const QString& searchText) {
    if (searchText.isEmpty()) return;

    QString text = label->text();

    if (text.toLower().contains(searchText)) {
        QString escapedSearch = QRegularExpression::escape(searchText);
        QRegularExpression regex(escapedSearch, QRegularExpression::CaseInsensitiveOption);

        QString highlighted = text.replace(regex,
                                           QString("<span style='background-color: #FFEB3B;'>%1</span>").arg(searchText));

        label->setText(highlighted);
    }
}

void SettingsWindow::showAllItems() {
    QTreeWidgetItemIterator it(treeWidget);
    while (*it) {
        (*it)->setHidden(false);
        (*it)->setBackground(0, QColor(Qt::transparent));

        if (originalExpansionState.contains(*it)) {
            (*it)->setExpanded(originalExpansionState[*it]);
        }
        ++it;
    }

    for (auto it = groupPages.begin(); it != groupPages.end(); ++it) {
        QScrollArea* scrollArea = it.value();
        QWidget* contentWidget = scrollArea->widget();
        if (contentWidget) {
            QList<QLabel*> labels = contentWidget->findChildren<QLabel*>();
            for (QLabel* label : labels) {
                QString text = label->text();
                if (text.contains("<span")) {
                    text = text.remove(QRegularExpression("<[^>]*>"));
                    label->setText(text);
                }
            }

            QList<QWidget*> widgets = contentWidget->findChildren<QWidget*>();
            for (QWidget* widget : widgets) {
                widget->setVisible(true);
            }
        }
    }

    originalExpansionState.clear();
}

void SettingsWindow::loadSettings() {
    QSettings settings("TestLabs", "TestSettings");

    QList<SettingsItem*> allItems = rootItem->getAllChildren();
    allItems.prepend(rootItem);

    for (SettingsItem* item : allItems) {
        if (!item->isSavingEnabled() || item->isGroup()) continue;

        QString settingsPath = buildSettingsPath(item);
        QVariant savedValue = settings.value(settingsPath);

        if (savedValue.isValid()) {
            applyValueToWidget(item, savedValue);
        }
    }
}

void SettingsWindow::saveSettings() {
    QSettings settings("TestLabs", "TestSettings");

    QList<SettingsItem*> allItems = rootItem->getAllChildren();
    allItems.prepend(rootItem);

    for (SettingsItem* item : allItems) {
        if (!item->isSavingEnabled() || item->isGroup()) continue;

        QString settingsPath = buildSettingsPath(item);
        settings.setValue(settingsPath, item->getValue());
    }
    settings.sync();
}

QString SettingsWindow::buildSettingsPath(SettingsItem* item) const {
    QStringList pathParts;
    SettingsItem* current = item;

    while (current && current != rootItem) {
        if (!current->id().isEmpty()) {
            pathParts.prepend(current->id());
        }
        current = current->parent();
    }

    return pathParts.join("/");
}

void SettingsWindow::applyValueToWidget(SettingsItem* item, const QVariant& value) {
    if (QComboBox* comboBox = item->comboBox()) {
        int index = comboBox->findText(value.toString());
        if (index >= 0) comboBox->setCurrentIndex(index);
    } else if (QCheckBox* checkBox = item->checkBox()) {
        checkBox->setChecked(value.toBool());
    } else if (QSpinBox* spinBox = item->spinBox()) {
        spinBox->setValue(value.toInt());
    } else if (QLineEdit* lineEdit = qobject_cast<QLineEdit*>(item->controlWidget())) {
        lineEdit->setText(value.toString());
    } else if (QWidget* compositeWidget = item->controlWidget()) {
        QLineEdit* lineEdit = compositeWidget->findChild<QLineEdit*>();
        if (lineEdit) lineEdit->setText(value.toString());
    }
}

void SettingsWindow::connectSignalsForAutoSave() {
    QList<SettingsItem*> allItems = rootItem->getAllChildren();
    allItems.prepend(rootItem);

    for (SettingsItem* item : allItems) {
        if (!item->isSavingEnabled() || item->isGroup()) continue;

        if (QComboBox* comboBox = item->comboBox()) {
            connect(comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
                    this, &SettingsWindow::saveSettings);
        } else if (QCheckBox* checkBox = item->checkBox()) {
            connect(checkBox, &QCheckBox::toggled,
                    this, &SettingsWindow::saveSettings);
        } else if (QSpinBox* spinBox = item->spinBox()) {
            connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged),
                    this, &SettingsWindow::saveSettings);
        } else if (QLineEdit* lineEdit = qobject_cast<QLineEdit*>(item->controlWidget())) {
            connect(lineEdit, &QLineEdit::textChanged,
                    this, &SettingsWindow::saveSettings);
        } else if (QWidget* compositeWidget = item->controlWidget()) {
            QLineEdit* lineEdit = compositeWidget->findChild<QLineEdit*>();
            if (lineEdit) {
                connect(lineEdit, &QLineEdit::textChanged,
                        this, &SettingsWindow::saveSettings);
            }
        }
    }
}

void SettingsWindow::closeEvent(QCloseEvent* event) {
    saveSettings();
    event->accept();
}
