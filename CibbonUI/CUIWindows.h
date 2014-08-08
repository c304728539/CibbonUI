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
		virtual ~Context() = default;
	};

	class TextContext:public Context
	{
	public:
		cint TextColor;
		//Others ?
	};

	class Component
	{
	public:
		virtual void Draw(Context* p) = 0;
		virtual void Add(Component* p) = 0;
		virtual void Remove(Component* p) = 0;
		virtual cuiRect GetEventPosition() const
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

	class Container : public Composite ,public Subject,public Observer
	{
	public:
		virtual ~Container() = default;
	};

	class Canvas :public Container
	{
	public:
		void Draw(Context* p) override;
		void Add(Component* p, const cuiRect& cr);
		void SetRoot(CUIWindowBase* p);
		void SetPosition(const cuiRect& cr) override;
		virtual void LeftButtonDown(MouseEventArgs mea) override;
		virtual void LeftButtonUp(MouseEventArgs mea) override;
		virtual void RightButtonDown(MouseEventArgs mea) override;
		virtual void RightButtonUp(MouseEventArgs mea) override;
		virtual void MouseMove(MouseEventArgs mea) override;
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
	};


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
		
		cuiRect GetWindowRect() const;
		cuiRect GetClientRect() const;
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
		//MessageHandler* pHandler;
		std::wstring WindowTitle;
		HWND WindowHandle;
		std::shared_ptr<RenderManager> pRenderManager;

		virtual void CreateInitors() = 0;
		
	};

	class CUIMainWindow :public CUIWindowBase
	{
	public:

		CUIMainWindow(const std::wstring& Title);
		//void Run() override;
		bool HandleMessage(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam, LRESULT& lre);
		void RunMessageLoop() override;
		void SetContainer(Container* _pContainer);
		void SetThings() override;
		void Draw(Context* p);
		void Add(Container* p);
		virtual ~CUIMainWindow();
	protected:

		virtual void CreateInitors() override;
	private:
		Container* pContainer;
		MainWindowMessageHandler* pMainWindowMessageHandler;
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

	class FactoriesCreator
	{
	public:
		const cint NormalFontSize = 12;
		ID2D1Factory* GetpD2DFactory() const;
		IDWriteFactory* GetpDwFactory() const;
		IDWriteTextFormat* GetpNormalTextFormat();
		static const std::unique_ptr<FactoriesCreator>& GetFactoriesCreator() ;
		~FactoriesCreator();
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
		using DwriteTextAlignment = DWRITE_TEXT_ALIGNMENT;

		const DwriteTextAlignment TextAlignmentCenter = DWRITE_TEXT_ALIGNMENT_CENTER;
		const DwriteTextAlignment TextAlignmentLeft = DWRITE_TEXT_ALIGNMENT_LEADING;
		const DwriteTextAlignment TextAlignmentRight = DWRITE_TEXT_ALIGNMENT_TRAILING;

		using DwriteParagraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT;

		const DwriteParagraphAlignment ParagraphAlignmentTop = DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
		const DwriteParagraphAlignment ParagraphAlignmentCenter = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
		const DwriteParagraphAlignment ParagraphAlignmentBottom = DWRITE_PARAGRAPH_ALIGNMENT_FAR;

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

		IDWriteTextFormat* GetpTextFormat(cint FontSize, const std::wstring& FontName, DwriteTextAlignment, DwriteParagraphAlignment);
	};

	class EventHandler
	{
	public:
		//EventHandler(CUIControl* pControl);
		virtual void LeftButtonDown(MouseEventArgs mea) = 0;
		virtual void LeftButtonUp(MouseEventArgs mea) = 0;
		virtual void RightButtonDown(MouseEventArgs mea) = 0;
		virtual void RightButtonUp(MouseEventArgs mea) = 0;
		virtual void MouseMove(MouseEventArgs mea) = 0;
	protected:
		
	};


	class ButtonEventHandler : public EventHandler
	{
	public:
		ButtonEventHandler(Win8Button* pControl);
		virtual void LeftButtonDown(MouseEventArgs mea) override;
		virtual void LeftButtonUp(MouseEventArgs mea) override;
		virtual void RightButtonDown(MouseEventArgs mea) override;
		virtual void RightButtonUp(MouseEventArgs mea) override;
		virtual void MouseMove(MouseEventArgs mea) override;
		cuiRect GetPosition() const;
		void SetPosition(const cuiRect& pos);
	private:
		Win8Button* pButton;
		cuiRect Position;
	};

	enum Statesenum
		{
			Click,
			MoveOn,
			Normal,
			Disabled
			//Other States Maybe
		};
	class CUIControl : public Observer, public Composite
	{
	public:
		CUIControl();
		//CUIControl(CUIWindowBase* pWindow);
		void SetRoot(CUIWindowBase* p) override
		{
			pOwnerWindow = p;
		}
		virtual Statesenum GetState() = 0;
		virtual void SetState(Statesenum _state) = 0;
		virtual ~CUIControl() = default;
		virtual void Update();
		virtual cuiRect GetEventPosition() const = 0;
		//virtual void SetEventPosition(const cuiRect& pos) = 0;
		void SetpOwnerWindow(CUIWindowBase* p);
	private:
		CUIWindowBase* pOwnerWindow;

	};




	class Win8Button :public CUIControl
	{
	public:
		
		Win8Button(const std::wstring& ButtonContent = L"");
		virtual void LeftButtonDown(MouseEventArgs mea);
		virtual void LeftButtonUp(MouseEventArgs mea);
		virtual void RightButtonDown(MouseEventArgs mea);
		virtual void RightButtonUp(MouseEventArgs mea);
		virtual void MouseMove(MouseEventArgs mea);
		virtual cuiRect GetEventPosition() const override;
		virtual Statesenum GetState() override;
		virtual void SetState(Statesenum _state) override;
		virtual void Draw(Context* p) override;
		void SetPosition(const cuiRect& pos) override;
		virtual void Update() override;
		~Win8Button();
	private:
		Statesenum ButtonState;
		ButtonCompositeFactory* pNormalButtonFactory;
		ButtonCompositeFactory* pMoveOnButtonFactory;
		ButtonCompositeFactory* pClickButtonFactory;
		ButtonCompositeFactory* pNowButtonFactory;
		ButtonEventHandler* pEventHandler;
		std::wstring ButtonContent;
		

	};

	class Border : public Leaf
	{
	public:
		Border(cint _BorderColor, float _BorderWidth = 1.f);
		void Draw(Context* p) override;
	private:
		//cuiRect BorderArea;
		cint BorderColor;
		float BorderWidth;
	};

	class Inside :public Leaf
	{
	public:
		Inside(cint color1, cint color2);
		void Draw(Context* p) override;
	private:
		//cuiRect InsideArea;
		cint InsideColor1;
		cint InsideColor2;
	};

	class Content :public Leaf
	{
	public:
		Content(const std::wstring& _text);
		void Draw(Context* p) override;
		const std::wstring& GetContent() const;
	private:
		cuiRect ContentArea;
		std::wstring ContentString;
		cint ContentLength;
		cint ContentWidth;
	};

	class ButtonCompositeFactory
	{
	public:
		ButtonCompositeFactory();
		virtual Border* CreateBorder() = 0;
		virtual Inside* CreateInside() = 0;
		virtual Content* CreateContent(const std::wstring& _text) = 0;
		virtual ~ButtonCompositeFactory();
	protected:
		Border* pBorder;
		Inside* pInside;
		Content* pContent;
	};


	class NormalButtonCompositeFactory : public ButtonCompositeFactory
	{
	public:
		Border* CreateBorder();
		Inside* CreateInside();
		Content* CreateContent(const std::wstring& _text) override;
	};

	class MoveOnButtonCompositeFactory : public ButtonCompositeFactory
	{
	public:
		Border* CreateBorder();
		Inside* CreateInside();
		Content* CreateContent(const std::wstring& _text) override;
	};

	class ClickButtonCompositeFactory : public ButtonCompositeFactory
	{
	public:
		Border* CreateBorder();
		Inside* CreateInside();
		Content* CreateContent(const std::wstring& _text) override;
	};


}