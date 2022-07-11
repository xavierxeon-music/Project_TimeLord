#include "SketchWidget.h"

#include <QFileDialog>
#include <QStatusBar>
#include <QToolBar>
#include <QToolButton>
#include <QVBoxLayout>

#include "DelegateSpinBox.h"
#include "SketchModel.h"

Sketch::Widget::Widget(QWidget* widget, Model* model)
   : QDialog(widget)
   , model(model)
   , stateTree(nullptr)
{
   setWindowTitle("Sketch / Time Lord UI");
   setMinimumSize(500, 800);

   stateTree = new QTreeView(this);
   stateTree->setModel(model);
   connect(stateTree, &QTreeView::doubleClicked, this, &Widget::slotItemDoubleClicked);
   stateTree->setItemDelegateForColumn(2, new Delegate::SpinBox(this, 1000));

   QToolBar* toolBar = new QToolBar(this);
   {
      QAction* createAction = new QAction(QIcon(":/Add.svg"), "Create", this);
      connect(createAction, &QAction::triggered, this, &QDialog::accept);
      QToolButton* createButton = new QToolButton();
      createButton->setDefaultAction(createAction);
      createButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
      toolBar->addWidget(createButton);

      toolBar->addSeparator();
      toolBar->addAction(QIcon(":/Break.svg"), "Add Break", this, &Widget::slotAddBreak);
      toolBar->addSeparator();
      toolBar->addAction(QIcon(":/LoadFromFile.svg"), "Load From File", this, &Widget::slotLoadFromFile);
      toolBar->addAction(QIcon(":/SaveNewFile.svg"), "Save To New File", this, &Widget::slotSaveNewFile);

      QWidget* stretch = new QWidget(this);
      stretch->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
      toolBar->addWidget(stretch);

      toolBar->addSeparator();
      toolBar->addAction(QIcon(":/Remove.svg"), "Reject", this, &QDialog::reject);
   }

   QStatusBar* statusBar = new QStatusBar(this);
   statusBar->showMessage(model->compileInfo());

   QVBoxLayout* masterLayout = new QVBoxLayout(this);
   masterLayout->setSpacing(0);
   masterLayout->setContentsMargins(0, 0, 0, 0);
   masterLayout->addWidget(toolBar);
   masterLayout->addWidget(stateTree);
   masterLayout->addWidget(statusBar);
}

void Sketch::Widget::slotLoadFromFile()
{
   model->loadFromFile();
}

void Sketch::Widget::slotSaveNewFile()
{
   model->saveToFile();
}

void Sketch::Widget::slotAddBreak()
{
   model->addBreak();
}

void Sketch::Widget::slotItemDoubleClicked(const QModelIndex& index)
{
   if (!index.isValid())
      return;

   model->sendItemState(index);
}
