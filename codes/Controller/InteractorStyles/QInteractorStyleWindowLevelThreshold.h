#ifndef __QINTERACTOR_STYLE_WINDOW_LEVEL_H__
#define __QINTERACTOR_STYLE_WINDOW_LEVEL_H__
/**
* @file	QInteractorStyleWindowLevelThreshold.h
* @brief	whatever
* @authour	wuzhuobin
* @date	20/12/2016
* @version	v1.0.0
*
* <pre>copyright:	authors have all right reserved. <pre/>
* <pre>email:		jiejin2022@163.com. <pre/>
* <pre>company:	CUHK. <pre/>
*/
#include "QAbstractNavigation.h"
#include "InteractorStyleWindowLevelThreshold.h"

namespace Ui {class QInteractorStyleWindowLevelThreshold;}
/**
* @class	QInteractorStyleWindowLevelThreshold
* @brief	window level.
* using cursor to change window level.
*/
class QSpinBox;
class QSlider;
class QPushButton;
class QLabel;
class QInteractorStyleWindowLevelThreshold : public QAbstractNavigation,
	public InteractorStyleWindowLevelThreshold
{
	Q_OBJECT;
	QSETUP_UI_HEAD(QInteractorStyleWindowLevelThreshold);

public:
	vtkTypeMacro(QInteractorStyleWindowLevelThreshold, InteractorStyleWindowLevelThreshold)
	static QInteractorStyleWindowLevelThreshold* New();
	/**
	* @brief	Enabled/Disable this InteractorStyle.
	* @param	flag	true, enable. false, disable.
	*/
	virtual void SetCustomEnabled(bool flag);
	/**
	* @override
	* @brief	function to set the focal point.
	* @param	ijk	int[3] array to set the focal point
	* @param	i, j, k	to set the focal point
	*/
	virtual void SetCurrentFocalPointWithImageCoordinate(int i, int j, int k);

	public slots:
	/**
	 * @override
	 * @brief	function to set the window width spinbox
	 */
	virtual void SetWindow(int window);

	/**
	* @override
	* @brief	function to set the window level spinbox
	*/
	virtual void SetLevel(int level);

	virtual void ThresholdTargetViewerToOverlay();
	virtual void SetOutputLabel(int label)override;



protected:

	/**
	* @brief	Constructor.
	* @param	uiType	NO_UI
	* @param	parent	QWidget's parent
	*/
	QInteractorStyleWindowLevelThreshold(int uiType = UNIQUE_UI, QWidget * parent = Q_NULLPTR);
	/**
	 * @brief	Destructor. 
	 */
	virtual ~QInteractorStyleWindowLevelThreshold();
	virtual void uniqueInvoke(bool flag);
	/**
	* @override
	* @brief	abstract funtion to call once.
	* @see #uniqueInvoke()
	*
	* The #uniqueCall() function used to guarantee only call once.
	* For updating the spinbox's boundaries.
	*/
	virtual void uniqueCall();
	virtual void uniqueEnable() override;
	virtual void SetWindowByViewer(double window);
	virtual void SetLevelByViewer(double level);

	void UpdateTargetViewer();


	protected slots:
	virtual void ResetWindowLevel() override;



private:
	void initialization();
	void uniqueInitialization();
	///< the pointer of the navigation ui.
	Ui::QInteractorStyleWindowLevelThreshold *ui = nullptr;
	QLabel* m_label = nullptr;
	QSpinBox* m_spinBoxWindowLevel = nullptr;
	QSpinBox* m_spinBoxWindowWidth = nullptr;
	QSlider* m_sliderWindowLevel = nullptr;
	QSlider* m_sliderWindowWidth = nullptr;
	QPushButton* m_pushButtonReset = nullptr;
	QStringList m_listOfModalityNames;

};


#endif // !__QINTERACTOR_STYLE_WINDOW_LEVEL_H__
