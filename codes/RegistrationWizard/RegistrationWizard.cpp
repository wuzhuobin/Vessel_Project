#include "RegistrationWizard.h"
#include <QLabel>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QSpacerItem>
#include <QProgressBar>
#include <QTreeWidgetItem>
#include <QDirIterator>
#include <QTabWidget>
#include <QTableWidget>
//#include <QSpinBox>
#include <QDebug>
#include <QStringList>
#include <QString>
#include <QTableWidgetItem>
#include <QFileInfo>
#include <qheaderview.h>

#include "itkGDCMSeriesFileNames.h"
#include "itkImageSeriesReader.h"
#include "itkGDCMImageIO.h"



RegistrationWizard::RegistrationWizard(
	QString dir, int numOfImages, QWidget *parent)
	: m_numOfImages(numOfImages), QWizard(parent)
{
	directoryPage	= new DirectoryPage(dir, this);
	imagePage		= new ImagePage(numOfImages, this);
	conclusionPage	= new ConclusionPage(numOfImages, this);

	for (int i = 0; i < numOfImages; ++i) {
		m_selectedImages << -1;
	}

	//imagePage->m_DICOMHeaders = &m_DICOMHeaders;
	imagePage->m_imagePaths = &m_imagePaths;
	imagePage->m_selectedImages = &m_selectedImages;

	//conclusionPage->m_DICOMHeaders = &m_DICOMHeaders;
	conclusionPage->m_imagePaths = &m_imagePaths;
	conclusionPage->m_selectedImages = &m_selectedImages;

    setPage(PAGE_DIRECTORY,  directoryPage);
    setPage(PAGE_IMAGE,		 imagePage );
    setPage(PAGE_CONCLUSION, conclusionPage);
	
    setStartId(PAGE_DIRECTORY);
    setWindowTitle(tr("  Registration Wizard  "));
	//setMinimumSize(800, 800);
	setOption(HaveHelpButton,	true);
	setOption(NoCancelButton,	true);
	setOption(IndependentPages,	false);


    connect(this, SIGNAL(helpRequested()), this, SLOT(showHelp()));

	//directoryPage->setDirectory(dir);
}

RegistrationWizard::RegistrationWizard(int numOfImages, QWidget * parent)
	:RegistrationWizard(QString(), numOfImages, parent)
{
}

RegistrationWizard::RegistrationWizard(QWidget * parent)
	: RegistrationWizard(2, parent)
{
}

RegistrationWizard::~RegistrationWizard()
{
	// clear the m_fileNamesList
	m_fileNamesList.clear();
}

void RegistrationWizard::setImageModalityNames(unsigned int i, QString imageModalityName)
{
	imagePage->setImageModalityNames(i, imageModalityName);
}

void RegistrationWizard::showHelp()
{
    QString message;

    switch (currentId()) {
    case PAGE_DIRECTORY:
        message = tr("Select the image directory");
        break;
    case PAGE_IMAGE:
        message = tr("Select the image series");
        break;
    case PAGE_CONCLUSION:
        message = tr("Confirm the image information");
        break;
    default:
        message = tr("");
    }

    QMessageBox::information(this, tr("Registration Wizard Help"), message);

}

QStringList* RegistrationWizard::getFileNames(unsigned int i)
{
	if (i<0 || i >= m_fileNamesList.size())
		return nullptr;
	else
		return m_fileNamesList[i].data();
}

const QString RegistrationWizard::getDirectory()
{
	return field("directory.path").toString();
}

int RegistrationWizard::getNumberOfImages()
{
	return m_numOfImages;
}

DirectoryPage::DirectoryPage(QString dir, QWidget *parent)
	: QWizardPage(parent)
{
	setTitle(tr("Set Image Directory"));
	setSubTitle(tr(""));

	m_imageDirectoryLineEdit = new QLineEdit(this);
	m_imageDirectoryLineEdit->setText(dir);
	registerField("directory.path*", m_imageDirectoryLineEdit);

	m_imageDirectoryLabel = new QLabel(tr("&Image Directory:"), this);
	m_imageDirectoryLabel->setBuddy(m_imageDirectoryLineEdit);

	browseBtn = new QPushButton("Browse", this);


	QHBoxLayout* hBoxLayout = new QHBoxLayout(this);
	hBoxLayout->addWidget(m_imageDirectoryLabel);
	hBoxLayout->addWidget(m_imageDirectoryLineEdit);
	hBoxLayout->addWidget(browseBtn);
	setLayout(hBoxLayout);

	connect(browseBtn, SIGNAL(clicked()), this, SLOT(browse()));
}

DirectoryPage::DirectoryPage(QWidget *parent)
    : DirectoryPage(QString(), parent)
{

}

void DirectoryPage::browse()
{
	QDir dir = QDir(m_imageDirectoryLineEdit->text());
	if (!dir.exists()) {
		dir = QDir();
	}

	//Dialog Open Directory
	QString path = QFileDialog::getExistingDirectory(this,
		tr("Open Image Directory"), dir.absolutePath(), 
		QFileDialog::ShowDirsOnly);
	path.replace("\\","/");
	m_imageDirectoryLineEdit->setText(path);
}

int DirectoryPage::nextId() const
{
    return RegistrationWizard::PAGE_IMAGE;
}

void DirectoryPage::setDirectory(QString dir)
{
	m_imageDirectoryLineEdit->setText(dir);
}

ImagePage::ImagePage(int numOfImages, QWidget *parent)
	: QWizardPage(parent)
{
	setTitle(tr("Select image series"));

	//Tree
	QStringList headerList;

	headerList << "Patient Name" << "Modality" << "Series Description" <<
		"Slice" << "Date" << "Series UID" << "Format" << "Path";

	treeWidget = new QTreeWidget(this);
	treeWidget->setHeaderLabels(headerList);
	treeWidget->setMinimumSize(1000, 400);

	progressBar = new QProgressBar(this);
	progressBar->setRange(0, 100);
	progressBar->setTextVisible(true);
	progressBar->setStyleSheet("QProgressBar {text-align: center;}");


	QVBoxLayout* vBoxLayout = new QVBoxLayout(this);
	vBoxLayout->addWidget(treeWidget);
	vBoxLayout->addSpacerItem(new QSpacerItem(50, 1));
	vBoxLayout->addWidget(progressBar);
	setLayout(vBoxLayout);

	// initialization 
	for (int i = 0; i < numOfImages; ++i) {
		m_imageLineEdits << new QLineEdit(this);
		m_imageLineEdits[i]->setReadOnly(true);

		m_imageLabels << new QLabel(this);
		m_imageLabels[i]->setBuddy(m_imageLineEdits[i]);
		

		m_imageSetBtns <<new QPushButton("<<", this);
		m_imageSetBtns[i]->setFixedSize(30, 30);

		m_imageRemoveBtns << new QPushButton("X", this);
		m_imageRemoveBtns[i]->setFixedSize(30, 30);

		//m_imageSpinBoxs << new QSpinBox(this);
		//Set Initial Value to -ve value
		//m_imageSpinBoxs[i]->setMinimum(-1);
		//m_imageSpinBoxs[i]->setValue(-1);

		QHBoxLayout* hBoxLayout = new QHBoxLayout();
		hBoxLayout->addWidget(m_imageLabels[i]);
		hBoxLayout->addWidget(m_imageLineEdits[i]);
		hBoxLayout->addWidget(m_imageSetBtns[i]);
		hBoxLayout->addWidget(m_imageRemoveBtns[i]);
		//hBoxLayout->addWidget(m_imageSpinBoxs[i]);
		vBoxLayout->addLayout(hBoxLayout);

		//* mean compulsory , otherwise optional 
		registerField("image.set" + QString::number(i), m_imageLineEdits[i]);

		connect(m_imageSetBtns[i], SIGNAL(clicked()), this, SLOT(setImages()), Qt::UniqueConnection);
		connect(m_imageRemoveBtns[i], SIGNAL(clicked()), this, SLOT(removeImages()), Qt::UniqueConnection);

	}

	//Initialize 
	thread = new FindImageThread(this);
	thread->setImagePage(this);
	lastDirectory = "";

	connect(thread, SIGNAL(updateProgressBar(int)), this->progressBar, SLOT(setValue(int)));

}

ImagePage::ImagePage(QWidget *parent)
	:ImagePage(2, parent)
{
    
}

void ImagePage::setImageModalityNames(unsigned int i, QString imageModalityName)
{
	if (i < 0 || i >= m_imageLabels.size())
		return;
	else
		m_imageLabels[i]->setText(imageModalityName);
}

bool ImagePage::validatePage()
{
	for (int i = 0; i < m_imageLineEdits.size(); ++i) {
		if (!m_imageLineEdits[i]->text().isEmpty())
			break;
		else if (i == m_imageLineEdits.size()-1) {
			QMessageBox::critical(this, "No image was selected. ", 
				"Please select at least one image.");
			return false;
		}
	}
	return true;
}

void ImagePage::initializePage()
{
	//Find Images	
	if (lastDirectory != field("directory.path").toString())
	{		
		while (thread->isRunning())
		{
			thread->forceStop();
			//thread->wait(500);
		}
		lastDirectory = field("directory.path").toString();
		treeWidget->clear();
		//for (int i = 0; i < m_imageLineEdits.size(); ++i) {
		//	m_imageLineEdits[i]->clear();
		//}
		//((RegistrationWizard*)(wizard()))->fileNameList->clear();
		progressBar->setValue(0);
		thread->setDirectory(field("directory.path").toString());
		thread->start();
	}
}

int ImagePage::nextId() const
{
	return RegistrationWizard::PAGE_CONCLUSION;
}

void ImagePage::onUpdateProgressBar(int value)
{	
	progressBar->setValue(value);
}

void ImagePage::setImages()
{
	QPushButton* setButton = dynamic_cast<QPushButton*>(sender());
	if (setButton == nullptr) 
		return;

	int index = m_imageSetBtns.indexOf(setButton);
	if (index < 0)
		return;
	else {
		QTreeWidgetItem* treeWidgetItem = treeWidget->currentItem();
		QString text(
			"NAME: " + treeWidgetItem->text(0) + 
			"	DESCRIPTION: " + treeWidgetItem->text(2));
		m_imageLineEdits[index]->setText(text);
		(*m_selectedImages)[index] = treeWidget->indexOfTopLevelItem(treeWidgetItem);
	}

}

void ImagePage::removeImages()
{
	QPushButton* setButton = dynamic_cast<QPushButton*>(sender());
	if (setButton == nullptr)
		return;

	int index = m_imageRemoveBtns.indexOf(setButton);
	if (index < 0)
		return;
	else {
		m_imageLineEdits[index]->clear();
		(*m_selectedImages)[index] = -1;
	}


}

ConclusionPage::ConclusionPage(int numOfImages, QWidget *parent)
	: QWizardPage(parent)
{
	for (int i = 0; i < numOfImages; ++i) {
		m_tableWidgets << nullptr;
	}

	setTitle(tr("Image Properties"));

	bottomLabel = new QLabel;
	bottomLabel->setWordWrap(true);
	bottomLabel->setText(tr("Please check the image header information."));

	tabWidget = new QTabWidget(this);


	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(tabWidget);
	layout->addWidget(bottomLabel);
	setLayout(layout);
}


ConclusionPage::ConclusionPage(QWidget *parent)
    : ConclusionPage(2, parent)
{

}

int ConclusionPage::nextId() const
{
    return -1;
}

void ConclusionPage::initializePage()
{
	using itk::GDCMImageIO;
	using itk::MetaDataDictionary;
	using std::string;
	typedef itk::ImageFileReader<itk::Image<unsigned short, 2>> ImageFileReader;
	using itk::MetaDataObjectBase;
	typedef itk::MetaDataObject<string> MetaDataString;


	tabWidget->clear();
	

	for (int i = 0; i < m_tableWidgets.size(); ++i) {

		QString fieldString;
		fieldString += "image.set";
		fieldString += QString::number(i);

		if ((*m_selectedImages)[i] != -1) {
			if (m_tableWidgets[i] == nullptr) {
				m_tableWidgets[i] = new QTableWidget(this);
			}
			tabWidget->addTab(m_tableWidgets[i], 
				field(fieldString).toString());
			m_tableWidgets[i]->clear();
			m_tableWidgets[i]->setEditTriggers(QTableWidget::NoEditTriggers);
			m_tableWidgets[i]->setColumnCount(3);
			m_tableWidgets[i]->setRowCount(0);
			m_tableWidgets[i]->setHorizontalHeaderLabels(QStringList()
				<< "Tag" << "Label" << "Description");
			m_tableWidgets[i]->verticalHeader()->setVisible(false);
			
			try {
				GDCMImageIO::Pointer dicomIO = GDCMImageIO::New();
				ImageFileReader::Pointer reader = ImageFileReader::New();
				reader->SetFileName((*(*m_imagePaths)[(*m_selectedImages)[i]])[0].toStdString());
				qDebug() << (*(*m_imagePaths)[(*m_selectedImages)[i]])[0];
				reader->SetImageIO(dicomIO);
				reader->Update();

				const MetaDataDictionary& dictionary = dicomIO->GetMetaDataDictionary();
				for (MetaDataDictionary::ConstIterator cit = dictionary.Begin();
					cit != dictionary.End(); ++cit) {

					MetaDataObjectBase::Pointer entry = cit->second;
					MetaDataString::Pointer entryValue = dynamic_cast<MetaDataString*>(
						entry.GetPointer());

					if (entryValue) {
						m_tableWidgets[i]->insertRow(m_tableWidgets[i]->rowCount());
						string tag = cit->first;
						string label;
						string value = entryValue->GetMetaDataObjectValue();
						m_tableWidgets[i]->setItem(m_tableWidgets[i]->rowCount() - 1, 2,
							new QTableWidgetItem(QString::fromStdString(value)));
						m_tableWidgets[i]->setItem(m_tableWidgets[i]->rowCount() - 1, 0,
							new QTableWidgetItem(QString::fromStdString(tag)));
						if (GDCMImageIO::GetLabelFromTag(tag, label)) {
							m_tableWidgets[i]->setItem(m_tableWidgets[i]->rowCount() - 1, 1,
								new QTableWidgetItem(QString::fromStdString(label)));
						}
						else {
							m_tableWidgets[i]->setItem(m_tableWidgets[i]->rowCount() - 1, 1,
								new QTableWidgetItem(tr("Unknown")));
						}

					}
				}
			}
			catch (itk::ExceptionObject& e) {
				qDebug() << e.what();
			}

			//if (!(*m_DICOMHeaders)[i].isNull()) {
			//	int tableRow = 0;
			//	m_tableWidgets[i]->setRowCount((*m_DICOMHeaders).size());
			//	m_tableWidgets[i]->setColumnCount(2);
			//	for (QMap<QString, QString>::const_iterator cit = (*m_DICOMHeaders)[i]->constBegin();
			//		cit != (*m_DICOMHeaders)[i]->constEnd(); ++cit) {

			//		qDebug() << cit.key();
			//		qDebug() << cit.value();
			//		m_tableWidgets[i]->setItem(tableRow, 0, new QTableWidgetItem(cit.key()));
			//		m_tableWidgets[i]->setItem(tableRow, 1, new QTableWidgetItem(cit.value()));
			//		++tableRow;
			//	}
			//}

		}


	}

}

FindImageThread::FindImageThread(QObject *parent) 
	:QThread(parent)
{
	m_stop = false;
}

void FindImageThread::setDirectory(QString dir)
{
	m_dir = dir;
}

void FindImageThread::setImagePage(ImagePage * imagePage)
{
	m_imagePage = imagePage;
}

void FindImageThread::updateTreeWidget(QStringList* imagePath, QString format)
{
	using std::string;
	using itk::GDCMImageIO;
	typedef itk::ImageFileReader<itk::Image<unsigned short, 2>> ImageFileReader;

	string patientName = "/";
	string modality = "/";
	string seriesDescription = "/";
	string slice = "/";
	string date = "/";
	string seriesUID = "/";

	if (format == "DICOM") {

		GDCMImageIO::Pointer dicomIO = GDCMImageIO::New();

		ImageFileReader::Pointer imageFilerReader = ImageFileReader::New();
		imageFilerReader->SetNumberOfThreads(4);
		imageFilerReader->SetImageIO(dicomIO);
		imageFilerReader->SetFileName((*imagePath)[0].toStdString());
		imageFilerReader->Update();


		if (!dicomIO->GetValueFromTag(("0010|0010"), patientName)) {
			//patientName = "/";
		}
		if (!dicomIO->GetValueFromTag("0008|0060", modality)) {
			//modality = "/";
		}
		if (!dicomIO->GetValueFromTag("0008|103e", seriesDescription)) {
			//seriesDescription = "/";
		}

		if (!dicomIO->GetValueFromTag("0020|1002", slice)) {
			//slice = "/";
		}
		slice = imagePath->size();
		if (!dicomIO->GetValueFromTag("0008|0020", date)) {
			//date = "/";
		}
		if (!dicomIO->GetValueFromTag("0020|0052", seriesUID)) {
			//seriesUID = "/";
		}
	}
	else {
		patientName = QFileInfo((*imagePath)[0]).baseName().toStdString();
	}


	QTreeWidgetItem* treeWidgetItem = new QTreeWidgetItem(m_imagePage->treeWidget);
	treeWidgetItem->setText(0, QString::fromStdString(patientName));
	treeWidgetItem->setText(1, QString::fromStdString(modality));
	treeWidgetItem->setText(2, QString::fromStdString(seriesDescription));
	treeWidgetItem->setText(3, QString::number(imagePath->size()));
	treeWidgetItem->setText(4, QString::fromStdString(date));
	treeWidgetItem->setText(5, QString::fromStdString(seriesUID));
	//treeWidgetItem->setText(6, "DICOM");
	treeWidgetItem->setText(6, format);
	treeWidgetItem->setText(7, (*imagePath)[0]);


	m_imagePage->treeWidget->addTopLevelItem(treeWidgetItem);
}

void FindImageThread::run()
{
	using std::vector;
	using std::string;
	using itk::GDCMSeriesFileNames;

	m_stop = false;

	if(this->m_stop)
		return;

	QStringList allDir;
	allDir << (m_dir);
	QDirIterator it(m_dir, QDir::Dirs | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
	while (it.hasNext())
		allDir << it.next();
	m_imagePage->m_imagePaths->clear();
	for (int i=0;i<allDir.size();i++)
	{
		if(this->m_stop)
			return;
		// Find Dicom files
		{		
			// Generate a sequence of fileNames from a DICOM series
			GDCMSeriesFileNames::Pointer nameGenerator = GDCMSeriesFileNames::New();
			nameGenerator->SetUseSeriesDetails(true);
			nameGenerator->SetDirectory(allDir.at(i).toStdString().c_str());
			//nameGenerator->AddSeriesRestriction("0008|0021");
			nameGenerator->GetInputFileNames();
			//itk::Object::GlobalWarningDisplayOff();

			qDebug() << "The directory: ";
			qDebug() << allDir.at(i);
			qDebug() << "Contains the following DICOM Series: ";
			const vector<string>& seriesUID = nameGenerator->GetSeriesUIDs();

			vector<string>::const_iterator seriesEnd = seriesUID.end();
			for (vector<string>::const_iterator cit = seriesUID.cbegin();
				cit != seriesUID.cend(); ++cit) {
				qDebug() << QString::fromStdString(*cit);
				vector<string> fileNames = nameGenerator->GetFileNames(*cit);

				QSharedPointer<QStringList> imagePath = QSharedPointer<QStringList>(
					new QStringList());
				*m_imagePage->m_imagePaths << imagePath;

				// vector<string> push to QList<QStringList>
				for (vector<string>::const_iterator cit1 = fileNames.cbegin();
					cit1 != fileNames.cend(); ++cit1) {
					*imagePath << QString::fromStdString(*cit1);
				}
				updateTreeWidget(imagePath.data(), "DICOM");
			}

		}
		
		//Find Nifti file
		{
			QStringList filterList;
			filterList << "*.nii" << "*.nii.gz";
			QDir dir(allDir.at(i));
			QFileInfoList niiFileList = dir.entryInfoList(filterList);


			for (QFileInfoList::const_iterator cit = niiFileList.cbegin(); 
				cit != niiFileList.cend(); ++cit)
			{
				QSharedPointer<QStringList> imagePath = QSharedPointer<QStringList>(
					new QStringList());
				*m_imagePage->m_imagePaths << imagePath;
				*imagePath << cit->absoluteFilePath();

				updateTreeWidget(imagePath.data(), "NIFTI");
			}
		}
		emit updateProgressBar(double(i + 1) / double(allDir.size()) * 100);
	}
}

void FindImageThread::forceStop()
{
	m_stop = true;
}


