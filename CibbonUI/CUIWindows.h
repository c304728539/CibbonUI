#pragma once
#include "CUIBase.h"
#include <string>
#include <vector>

namespace cibbonui
{
	class Subject;
	class CUIWindowBase;
	class Composite;
	class RenderManager;
	class FactoriesCreator;
	class Win8Button;
	class ButtonCompositeFactory;
	class ButtonFactoriesCreator;
	class Button;
	class ButtonBorderFactory;
	class ButtonInsideFactory;
	class ButtonContentFactory;
	class Caption;
	class MessageHandler;
	class Controller;
	class WindowContent;

	struct EventArgs
	{
		Subject* Sender;//The Window Maybe..
	};

	//There will be many EventArgs like MouseEventArgs...

	//It's Here..

	struct MouseEventArgs : public EventArgs
	{
		//KeyDowns Maybe
		cuiPoint EventPosition;
	};
	//Observer Pattern
	
	class Observer
	{
	public:
		virtual void LeftButtonDown(MouseEventArgs mea) = 0;
		virtual void LeftButtonUp(MouseEventArgs mea) = 0;
		virtual void RightButtonDown(MouseEventArgs mea) = 0;
		virtual void RightButtonUp(MouseEventArgs mea) = 0;
		virtual void MouseMove(MouseEventArgs mea) = 0;
		//virtual void Draw();
		virtual ~Observer() = default;

	private:

	};

	class Subject
	{
	public:
		void RegisterObserver(Observer* o);
		void RemoveObserver(Observer* o);
		//void NotifyObservers(EventArgs* pe);
	protected:
		std::vector<Observer*> Observerlist;
	};

	class Context
	{
	public:
		Context(CUIWindowBase* p);
		cuiRect Position;
		CUIWindowBase* pWindow;
		Composite* pParent;
	};

	class Component
	{
	public:
		virtual void Draw(Context* p) = 0;
		virtual void Add(Component* p) = 0;
		virtual void Remove(Component* p) = 0;
		virtual cuiRect GetPosition() const
		{
			return cuiRect();
		}
		virtual void SetPosition(const cuiRect&)
		{

		}
		virtual void SetRoot(CUIWindowBase* p)
		{

		}
		virtual ~Component() = default;
		virtual const Component* GetComposite() const = 0;
		//virtual void GetChild(cint i) = 0;
	protected:
		//Composite* pRoot;
	};

	
	class Leaf : public Component
	{
	public:
		void Add(Component* p);
		void Remove(Component* p);
		//void GetChild(cint i);
		const Component* GetComposite() const override;
		virtual ~Leaf() = default;
	private:

	};

	class Composite :public Component
	{
	public:
		//void Draw(Context* p) = 0;
		void Add(Component* p) override;
		void Remove(Component* p) override;
		const Component* GetComposite() const override;
		virtual ~Composite() = default;
	protected:
		std::vector<Component*> Leaves;
	};

	class Container : public Composite 
	{
	public:
		virtual void ReSize(const cuiRect& _size) = 0;
		virtual ~Container()
		{
			for (auto p : Leaves)
			{
				delete p;
			}
		}
	};

	class Canvas :public Container
	{
	public:
		void Draw(Context* p) override;
		void Add(Component* p, const cuiRect& cr);
		void SetRoot(CUIWindowBase* p);
		void SetPosition(const cuiRect& cr) override;
		void ReSize(const cuiRect& _size) override;
	private:
		cuiRect Position;
	};

	

	/*class Canvas :public Composite,public Observer
	{

	};
*/
	class CUIWindowBase;
	//Strategy Pattern 
	//this class is used to register a window class
	class WndClass
	{
	public:
		WndClass(
			const std::wstring& _name,
			bool shadow,
			WNDPROC WndProc,
			HINSTANCE hInst
			);
		WndClass();
		std::wstring getClassName() const;
		void RegisterWindow();
	private:
		ATOM atom;
		WNDCLASSEX WndClassEx;
		std::wstring ClassName;
	};

	class WindowCreator
	{
	public:
		WindowCreator(const std::wstring& ClassName,const std::wstring& Title);
		void SetExStyle(DWORD _ExStyle);
		void SetWidth(cint _width);
		void SetHeight(cint _Height);
		void SetParentWindow(HWND hParent);
		void SetlpParam(LPVOID p);
		HWND Create();
	private:
		std::wstring WindowTitle;
		DWORD ExStyle;
		DWORD WindowStyle;
		cint Width;
		cint Height;
		HWND hParentWindow;
		LPVOID lpParam;
		std::wstring WindowClassName;
	};

	//MessageHandler is a Subject
	//A Mediator
	class MessageHandler
	{
	public:
		const bool Already = true;
		const bool NotYet = false;
		virtual bool MessageHandlerWndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam,LRESULT& lre) = 0;
		MessageHandler(CUIWindowBase* pWindow);
		virtual ~MessageHandler() ;
	protected:
		CUIWindowBase* pOwnerWindow;
		Context* pContext;
		std::shared_ptr<Controller> pController;
	};

	class MainWindowMessageHandler : public MessageHandler
	{
	public:
		MainWindowMessageHandler(CUIWindowBase* pOwnerWindow);
		bool MessageHandlerWndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam, LRESULT& lre) override;
		void SetContainer(Container* p);

	private:
		LRESULT HitTestNCA(HWND hWnd, WPARAM wParam, LPARAM lParam);
		Container* pContainer;
		HANDLE hRenderThread;
		bool Tracked;
	};
	class Controller : public Subject
	{
		friend class MainWindowMessageHandler;
	public:
		
		static std::shared_ptr<Controller> GetpController();
	private:
		Controller();
		static std::shared_ptr<Controller> pController;
	};

	unsigned int __stdcall RenderThreadFunc(void* p);

	LRESULT CALLBACK WndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
	class CUIWindowBase
	{
	public:
		 
		CUIWindowBase(const std::wstring& Title);
		virtual ~CUIWindowBase();
		void Initialize();
		void Run();
		virtual void SetThings() = 0;
		virtual void RunMessageLoop() = 0;
		const std::wstring& GetControlText() const
		{
			return	WindowTitle;
		}
		cuiRect GetWindowRect() const;
		cuiRect GetClientRect() const;
		virtual void ReSize(USHORT x, USHORT y)
		{};
		virtual void Draw(Context* p){};
		/*virtual void RegisterWindow(std::wstring _name,
			bool shadow,
			WNDPROC WndProc,
			HINSTANCE hInst) = 0;
		virtual void CreateInit(const std::wstring& ClassName, std::wstring Title) = 0;*/
		HWND GetWindowHandle() const;
		void ReDraw() const;
		std::shared_ptr<RenderManager> GetpRenderManager() const;
	protected:

		WndClass* pWndClass;
		WindowCreator* pWindowCreator;
		std::wstring WindowTitle;
		HWND WindowHandle;
		std::shared_ptr<RenderManager> pRenderManager;

		virtual void CreateInitors() = 0;
		
	};

	class CUIMainWindow :public CUIWindowBase
	{
	public:
		enum
		{
			CaptionHeight = 20
		};
		CUIMainWindow(const std::wstring& Title);
		//void Run() override;
		bool HandleMessage(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam, LRESULT& lre);
		void RunMessageLoop() override;
		void SetContainer(Container* _pContainer);
		void SetThings() override;
		void Draw(Context* p);
		void Add(Container* p);
		void ReSize(USHORT x1, USHORT x2);
		
		virtual ~CUIMainWindow();
	protected:

		virtual void CreateInitors() override;
	private:
		Container* pContainer;
		MainWindowMessageHandler* pMainWindowMessageHandler;
		Caption* pCaption;
		bool HasFirstSized;
		
	};


	/*Render APIs*/
	template<class Interface>
	inline void Free(Interface* & ppInterfaceToRelease)
	{
		if (ppInterfaceToRelease != nullptr)
		{
			ppInterfaceToRelease->Release();
			ppInterfaceToRelease = nullptr;
		}
	}
	using DwriteTextAlignment = DWRITE_TEXT_ALIGNMENT;

	const DwriteTextAlignment TextAlignmentCenter = DWRITE_TEXT_ALIGNMENT_CENTER;
	const DwriteTextAlignment TextAlignmentLeft = DWRITE_TEXT_ALIGNMENT_LEADING;
	const DwriteTextAlignment TextAlignmentRight = DWRITE_TEXT_ALIGNMENT_TRAILING;

	using DwriteParagraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT;

	const DwriteParagraphAlignment ParagraphAlignmentTop = DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
	const DwriteParagraphAlignment ParagraphAlignmentCenter = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
	const DwriteParagraphAlignment ParagraphAlignmentBottom = DWRITE_PARAGRAPH_ALIGNMENT_FAR;
	class FactoriesCreator
	{
	public:
		const cint NormalFontSize = 12;
		ID2D1Factory* GetpD2DFactory() const;
		IDWriteFactory* GetpDwFactory() const;
		IDWriteTextFormat* GetpNormalTextFormat();
		static const std::unique_ptr<FactoriesCreator>& GetFactoriesCreator() ;
		~FactoriesCreator();
		IDWriteTextFormat* GetpTextFormat(cint FontSize, const std::wstring& FontName, DwriteTextAlignment dta = TextAlignmentCenter, DwriteParagraphAlignment dpa = ParagraphAlignmentCenter);
	private:
		static std::unique_ptr<FactoriesCreator> pFactoriesCreator;
		FactoriesCreator();
		mutable ID2D1Factory* pD2DFactory;
		mutable IDWriteFactory* pDwFactory;
		mutable IDWriteTextFormat* pNormalTextFormat;
	};
	
	class RenderManager
	{
	public:
		

		static std::shared_ptr<RenderManager> GetpRenderManager(CUIWindowBase* p);
		~RenderManager();

		void BeginRender();
		void EndRender();
		void Resize(cint x, cint y);
		void Clear(cint ClearColor);
		void DrawBorder(const cuiRect& rect, cint BorderColor, float LineWidth);
		void NormalFillRect(const cuiRect& rect, cint FillColor);
		void LinearFillRect(const cuiRect& rect, cint FillColor1, cint FillColor2);
		void RenderText(const std::wstring& Content, const std::wstring& FontName, cint FontSize, const cuiRect& Area, DwriteTextAlignment, DwriteParagraphAlignment,cint TextColor);
		void RenderText(const std::wstring& Content , const cuiRect& Area,cint TextColor);
		void RenderText(const std::wstring& Content, const cuiRect& Area,cint TextColor,IDWriteTextFormat* pFormat);
		
		
		std::pair<float,float> CalTextSize();
	private:
		RenderManager(CUIWindowBase* p);

		ID2D1HwndRenderTarget* pRT;
		ID2D1Factory* pD2DFactory;
		IDWriteFactory* pDwFactory;
		static std::map<CUIWindowBase*, std::shared_ptr<RenderManager>> RenderManagerMap;
		std::unordered_map<cint, ID2D1SolidColorBrush*> SolidBrushMap;
		std::unordered_map<std::pair<cint, cint>, ID2D1LinearGradientBrush*> LinearBrushMap;
		ID2D1SolidColorBrush* GetBrush(cint BrushColor);
		
		ID2D1LinearGradientBrush* GetBrush(cint BrushColor1, cint BrushColor2, cuiRect _rect);
		IDWriteTextFormat* pNormalTextFormat;

		//IDWriteTextFormat* GetpTextFormat(cint FontSize, const std::wstring& FontName, DwriteTextAlignment, DwriteParagraphAlignment);
	};


	/******************************************************************
	*                                                                 *
	*                                                                 *
	*                   EventHandler                                  *
	*                                                                 *
	*                                                                 *
	*                                                                 *
	******************************************************************/
	class EventHandler
	{
	public:
		virtual void LeftButtonDown(MouseEventArgs mea) = 0;
		virtual void LeftButtonUp(MouseEventArgs mea) = 0;
		virtual void RightButtonDown(MouseEventArgs mea) = 0;
		virtual void RightButtonUp(MouseEventArgs mea) = 0;
		virtual void MouseMove(MouseEventArgs mea) = 0;
		virtual ~EventHandler() = default;
	protected:
		
	};


	class ButtonEventHandler : public EventHandler
	{
	public:
		ButtonEventHandler(Button* pControl);
		virtual void LeftButtonDown(MouseEventArgs mea) override;
		virtual void LeftButtonUp(MouseEventArgs mea) override;
		virtual void RightButtonDown(MouseEventArgs mea) override;
		virtual void RightButtonUp(MouseEventArgs mea) override;
		virtual void MouseMove(MouseEventArgs mea) override;
		cuiRect GetPosition() const;
		void OnClick(std::function<void()>);
		void DownClick(std::function<void()>);
		void SetPosition(const cuiRect& pos);
	private:
		Button* pButton;
		std::function<void()> func;
		std::function<void()> Downfunc;
	};


	/******************************************************************
	*                                                                 *
	*                                                                 *
	*                   Controls                                      *
	*                                                                 *
	*                                                                 *
	*                                                                 *
	******************************************************************/
	enum Statesenum
		{
			Click,
			MoveOn,
			Normal,
			Disabled,
			MoveOut
			//Other States Maybe
		};
	class CUIControl : public Observer, public Composite
	{
	public:
		CUIControl(EventHandler* p);
		void SetRoot(CUIWindowBase* p) override
		{
			pOwnerWindow = p;
		}
		virtual void LeftButtonDown(MouseEventArgs mea);
		virtual void LeftButtonUp(MouseEventArgs mea);
		virtual void RightButtonDown(MouseEventArgs mea);
		virtual void RightButtonUp(MouseEventArgs mea);
		virtual void MouseMove(MouseEventArgs mea);
		virtual void SetControlText(const std::wstring& _text) = 0;
		virtual const std::wstring& GetControlText() const = 0;
		virtual Statesenum GetState() = 0;
		virtual void SetState(Statesenum _state) = 0;
		virtual ~CUIControl();
		virtual void Update();
		virtual cuiRect GetPosition() const override;
		virtual void SetPosition(const cuiRect& cr) override;
		void SetpOwnerWindow(CUIWindowBase* p);
		CUIWindowBase* GetpOwnerWindow()
		{
			return pOwnerWindow;
		}
	protected:
		cuiRect Position;
		EventHandler* pEventHandler;
		CUIWindowBase* pOwnerWindow;
		bool Unshown;
	private:
		
		std::shared_ptr<Controller> pController;

	};

	class Button :public CUIControl
	{
	public:
		
		Button(
			ButtonBorderFactory* _pBorderFactory,
		   ButtonInsideFactory* _pInsidefactory,
		   ButtonContentFactory* _pContentFactory, 
		   const std::wstring& ButtonContent = L""
		);
		virtual Statesenum GetState() override;
		virtual void SetState(Statesenum _state) override;
		void SetControlText(const std::wstring& _text) override
		{
			ButtonContent = _text;
			Update();
		}
		const std::wstring& GetControlText() const override
		{
			return ButtonContent;
		}
		virtual void SetFont(const std::wstring& sw, cint _fontsize = 15, DwriteTextAlignment dta = TextAlignmentCenter, DwriteParagraphAlignment dpa = ParagraphAlignmentCenter);
		void Draw(Context* p) override;
		//virtual void Update() = 0 override;
		void OnClick(std::function<void()> foo);
		void DownClick(std::function<void()> foo);
		virtual ~Button();
	protected:
		Statesenum ButtonState;
		std::wstring ButtonContent;
		ButtonBorderFactory* pBorderFactory;
		ButtonInsideFactory* pInsideFactory;
		ButtonContentFactory* pContentFactory;
	};

	class Win8Button : public Button
	{
	public:
		Win8Button(const std::wstring& Content = L"");
		virtual void Update() override;
		//virtual void Draw(Context* p) override;
	};


	class FlatButton : public Button
	{
	public:
		FlatButton(const std::wstring& _Content = L"");
		virtual void Update() override;
	};

	class CapButton :public Button
	{
	public:
		CapButton(const std::wstring& ws = L"");
		virtual void Update() override;
		virtual void SetFont(const std::wstring& sw, cint _fontsize, DwriteTextAlignment dta = TextAlignmentCenter, DwriteParagraphAlignment dpa = ParagraphAlignmentCenter);
	private:
		WindowContent* pc;
	};

	class Border : public Leaf
	{
	public:
		Border();
		void Draw(Context* p) override;
		void SetBorderColor(cint _color);
		void SetBorderWidth(float _width = 1.f);
	private:
		cint BorderColor;
		float BorderWidth;
	};
	class Inside :public Leaf
	{
	public:
		virtual ~Inside() = default;
		virtual void SetInsideColor(cint color1, cint color2) = 0;
	};


	class LinearInside : public Inside
	{
	public:
		LinearInside();
		void SetInsideColor(cint _color1, cint _color2);
		void Draw(Context* p) override;
	private:
		cint LinearInsideColor1;
		cint LinearInsideColor2;
	};

	class PureInside : public Inside
	{
	public:
		PureInside();
		void SetInsideColor(cint _color, cint _color2) override;
		void Draw(Context* p) override;
	private:
		cint InsideColor;
	};

	class Content :public Leaf
	{
	public:
		Content();
		virtual ~Content() = default; 
		virtual void SetFont(const std::wstring& _fontname, cint fontsize, DwriteTextAlignment dta = TextAlignmentCenter, DwriteParagraphAlignment dpa = ParagraphAlignmentCenter) = 0;
		void SetContentColor(cint _color)
		{
			ContentColor = _color;
		}
	protected:
		cint ContentColor;
	};

	
	class PureContent :public Content
	{
	public:
		PureContent();
		void Draw(Context* p) override;
		void SetFont(const std::wstring& _fontname, cint fontsize, DwriteTextAlignment dta = TextAlignmentCenter, DwriteParagraphAlignment dpa = ParagraphAlignmentCenter) override
		{
			return;
		}
	};
    /*此Content用来绘制其它字体的文字，将pTextFormat保存好后就和PureContent一样了*/
	class SpecialContent : public Content
	{
	public:
		SpecialContent();
		void SetFont(const std::wstring& _fontname, cint fontsize, DwriteTextAlignment dta = TextAlignmentCenter, DwriteParagraphAlignment dpa = ParagraphAlignmentCenter) override;
		void Draw(Context* p);
		~SpecialContent();
	protected:
		IDWriteTextFormat* pFormat;
	};

	class WindowContent : public SpecialContent
	{
	public:
		void Draw(Context* p);
	};

	/******************************************************************
	*                                                                 *
	*                                                                 *
	*                   ScrollBar                                     *
	*                                                                 *
	*                                                                 *
	*                                                                 *
	******************************************************************/


	class ScrollBar : public CUIControl
	{
	public:
		//ScrollBar()
		void Draw(Context* p) override;
		void Add(Component* p, const cuiRect& cr);
		void SetRoot(CUIWindowBase* p);
		void SetPosition(const cuiRect& cr) override;
		cint GetCurrentPos() const;
		void SetCurrentPos(cint Pos);
		void SetMaxPos(cint _MinPos);
		void SetPage(cint _Page);
	private:
		cuiRect Position;
		cint MaxPos;
		const cint ScrollBarWidth = 20;
		cint CurrentPos;
		//int BarLength;
		
		int Page;
	};


	class ScrollBarEventHandler : public EventHandler
	{
	public:
		ScrollBarEventHandler(ScrollBar* pControl);
		virtual void LeftButtonDown(MouseEventArgs mea) override;
		virtual void LeftButtonUp(MouseEventArgs mea) override;
		virtual void RightButtonDown(MouseEventArgs mea) override;
		virtual void RightButtonUp(MouseEventArgs mea) override;
		virtual void MouseMove(MouseEventArgs mea) override;
		cuiRect GetPosition() const;
		void SetPosition(const cuiRect& pos);
	private:
		ScrollBar* pScrollBar;
		cuiRect Position;
		bool MouseHover;
	};


	const float Win8ButtonBorderWidth = 2.f;

	class ButtonBorderFactory
	{
	public:
		ButtonBorderFactory(Border* p);
		virtual Border* GetNormalBorder() = 0;
		virtual Border* GetMoveOnBorder() = 0;
		virtual Border* GetClickBorder() = 0;
		virtual~ButtonBorderFactory();
	protected:
		Border* pBorder;
	};

	class ButtonInsideFactory
	{
	public:
		ButtonInsideFactory(Inside* p);
		virtual Inside* GetNormalInside() = 0;
		virtual Inside* GetMoveOnInside() = 0;
		virtual Inside* GetClickInside() = 0;
		virtual ~ButtonInsideFactory();
	protected:
		Inside* pInside;
	};

	class ButtonContentFactory
	{
	public:
		ButtonContentFactory(Content* p);
		virtual Content* GetNormalContent() = 0;
		virtual Content* GetMoveOnContent() = 0;
		virtual Content* GetClickContent() = 0;
		void SetFont(const std::wstring& _text, cint fontsize = 15, DwriteTextAlignment dta = TextAlignmentCenter, DwriteParagraphAlignment dpa = ParagraphAlignmentCenter)
		{
			pContent->SetFont(_text, fontsize);
		}
		virtual~ButtonContentFactory();
	protected:
		Content* pContent;
	};

	class Win8ButtonBorderFactory : public ButtonBorderFactory
	{
	public:
		Win8ButtonBorderFactory();
		Border* GetNormalBorder() override;
		Border* GetMoveOnBorder();
		Border* GetClickBorder();
	};

	class Win8ButtonInsideFactory : public ButtonInsideFactory
	{
	public:
		Win8ButtonInsideFactory();
		Inside* GetNormalInside() override;
		Inside* GetMoveOnInside() override;
		Inside* GetClickInside() override;
	};

	class Win8ButtonContentFactory : public ButtonContentFactory
	{
	public:
		Win8ButtonContentFactory();
		Content* GetNormalContent();
		Content* GetMoveOnContent();
		Content* GetClickContent();
	
	};
	class FlatButtonInsideFactory : public ButtonInsideFactory
	{
	public:
		FlatButtonInsideFactory();
		Inside* GetNormalInside() override;
		Inside* GetMoveOnInside() override;
		Inside* GetClickInside() override;
	};
	class FlatButtonContentFactory : public ButtonContentFactory
	{
	public:
		FlatButtonContentFactory();
		Content* GetNormalContent();
		Content* GetMoveOnContent();
		Content* GetClickContent();
	};

	class SizeBox : public Container
	{
	public:
		SizeBox(CUIWindowBase* p);
		void Draw(Context* p) override;
		void ReSize(const cuiRect& _size) override;
		void SetRoot(CUIWindowBase* p) override;
		//~SizeBox();
	private:
		FlatButton* pMini;
		FlatButton* pClose;
		FlatButton* pMax;
	};

	class Caption : public Container
	{
	public:
		Caption(CUIWindowBase* p);
		void Draw(Context* p);
		void ReSize(const cuiRect& _size) override;
		void SetRoot(CUIWindowBase* p) override;
	private:
		SizeBox* pSizeBox;
		CapButton* pCap;
	};

}