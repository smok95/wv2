# WebView2 설치하기

WebView2는 MS에서 개발한 Chromium기반의 웹브라우저 컨트롤로

윈도우 네이티브앱에 내장하여 사용할 수 있도록 개발되었습니다.

![](https://docs.microsoft.com/en-us/microsoft-edge/webview2/media/webview2/what-webview.png#lightbox)

WebView2페이지 : https://docs.microsoft.com/en-us/microsoft-edge/webview2/

아마도 기존에 많이 사용하던 IE기반의 IWebBrowser2 컨트롤을 대체하기 위한 용도로 가장 많이 사용이 될 것 같습니다.



## 지원 OS

- Windows 11
- Windows 10
- Windows 8.1
- Windows 7 
- Windows Server 2019
- Windows Server 2016
- Windows Server 2012
- Windows Server 2012 R2
- Windows Server 2008 R2 





## 설치방법

IWebBrowser2의 경우에는 별도의 설치과정없이 사용할 수 있었지만,

WebView2의 경우에는 별도로 WebView2 Runtime이 설치가 되어 있어야만 사용할 수 있습니다.

(* 참고로 Edge dev/beta/canary 버전의 경우에는 WebView2 Runtime없이 WebView2를 사용할 수 있습니다.)



제가 헷갈렸던 부분이  윈도우7 이상에서  자동업데이트를 막아놓지 않은 이상 Chromium Edge가 자동을 설치되기 때문에 별도 설치없이 사용할 수 있는 줄 알았는데 그렇지 않았습니다.



WebView2는 Edge와는 전혀 별개로 WebView2 Runtime 위에서 동작합니다.

아마도 브라우저와 웹컨트롤의 사용자 환경을 완전히 분리시키기 위해서 이런식으로 구성을 한 것으로 보입니다.



그리고 Windows11부터는 WebView2 Runtime이 내장된다고 했던걸로 기억하는데요..

그렇다면 앞으로는 설치에 대한 고민은 안해도 될 것 같습니다.



### WebView2 Runtime 다운로드

WebView2 Runtime은 크게 2가지 버전으로 제공됩니다.

- Evergreen 버전

  처음 설치 이후에는 자동업데이트되어 최신버전으로 유지

- Fixed 버전

  고정버전으로 약 130MB 의 cab파일로 설치 및 업데이트는 직접 알아서 처리



Evergreen버전은 2가지 방식의 다운로드를 제공합니다.

- **Evergreen Bootstrapper**

  약 2MB의 인스톨러로 실행하면 runtime을 다운로드한 후에 설치가 되는 방식입니다.

- **Standalone Instanner**

  약 90MB의 인스톨러로 실행하면 바로 설치가 되는 방식입니다.



다운로드페이지 : https://developer.microsoft.com/en-us/microsoft-edge/webview2/#download-section



보시는 것처럼 인터넷 연결을 하지 않고 웹컨트롤을 사용해야하는 오프라인 환경이거나 버전을 특정하여 사용해야 하는 특수한 경우에는 Fixed버전을 사용하고

이외의 경우에는 Evergreen을 사용하면 될 것 같습니다.





















