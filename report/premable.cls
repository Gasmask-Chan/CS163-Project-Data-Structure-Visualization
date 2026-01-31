\usepackage{graphicx} % Required for inserting images
\usepackage[a4paper, top=1.6cm, bottom=2.3cm, left=1.4cm, right=1.4cm]{geometry}
\usepackage{csquotes}
\usepackage{listings}
\usepackage{xcolor}
\usepackage{tcolorbox}
\usepackage{titlesec}
\usepackage{etoolbox}
\usepackage{xparse}
\usepackage{amsmath}
\usepackage{tabularx}
\usepackage{lipsum}
\usepackage{parskip}
\usepackage{gensymb}
\usepackage{longtable}
\usepackage{hyperref}
\hypersetup{breaklinks=true}
\usepackage{xurl}
\usepackage{ulem}
\usepackage{amssymb}
\usepackage{minted}
\usemintedstyle{arduino}
\renewcommand{\ULdepth}{8pt}
\renewcommand{\arraystretch}{1.15}

\usepackage[T1]{fontenc}
\usepackage[vietnamese]{babel}
\usepackage{lastpage}
\setlength{\parindent}{0pt}

\newcolumntype{C}[1]{>{\centering\arraybackslash}p{#1}} % Custom fixed-width centered column

%==== FOOTER ====%
\usepackage{fancyhdr} % Custom headers and footers
\pagestyle{fancy}
\fancyhf{} % Clears default header/footer

\renewcommand{\footrulewidth}{1pt}
\renewcommand{\headrulewidth}{0pt}

%==== TABLE OF CONTENT ====%

\addto\captionsvietnamese{\renewcommand{\contentsname}{Table of Contents}}
\addto\captionsvietnamese{\renewcommand{\figurename}{Figure}}
\addto\captionsvietnamese{\renewcommand{\tablename}{Table}}
\addto\captionsvietnamese{\renewcommand{\refname}{References}}

%==== HYPERLINKS ====%

\hypersetup{
  colorlinks=true,       % Enable colored links instead of boxed
  linkcolor=blue,        % Color for internal links (like TOC)
  citecolor=blue,        % Color for citations
  urlcolor=blue,         % Color for URLs
}

%==== COLOR ====%

\definecolor{dkgreen}{rgb}{0,0.6,0}
\definecolor{gray}{rgb}{0.5,0.5,0.5}
\definecolor{mauve}{rgb}{0.58,0,0.82}
\definecolor{titleblue}{rgb}{0.0, 0.0, 0.0}
\definecolor{lightblue}{rgb}{0.0, 0.0, 0.0}

%==== CODE ====%
\lstset{frame=tb,
  language=C++,
  aboveskip=3mm,
  belowskip=3mm,
  showstringspaces=false,
  columns=flexible,
  basicstyle=\fontfamily{cmtt}\selectfont\small,
  numbers=none,
  numberstyle=\tiny\color{gray},
  keywordstyle=\color{blue},
  commentstyle=\color{dkgreen},
  stringstyle=\color{mauve},
  breaklines=true,
  breakatwhitespace=true,
  tabsize=3,
  texcl=true,
  numbers=left,
  numberstyle={\ttfamily\footnotesize\color{gray}}
}

%==== SECTIONS FORMATTING ====%

% Section
\newcommand{\sectionboxnumber}[1]{%
  \raisebox{-0.8mm}{%
    \tcbox[colframe=titleblue, colback=titleblue, colupper=white, nobeforeafter, arc=-2pt, top=0mm, bottom=0mm, left=1.5mm, right=1.5mm]{\small{\textbf\textsf{#1}}}%
  }%
}

\titleformat{\section}[hang]{\LARGE\bfseries\sffamily\color{titleblue}}{\sectionboxnumber{\thesection}}{4mm}{}[]

% Subsection
\newcommand{\subsectionboxnumber}[1]{%
  \raisebox{-1.1mm}{%
    \tcbox[colframe=lightblue, colback=lightblue, colupper=white, nobeforeafter, arc=-1pt, top=0mm, bottom=0mm, left=1mm, right=1mm,
    ]{\small\textsf{#1}}%
  }%
}

\titleformat{\subsection}[hang]{\Large\bfseries\sffamily}{\subsectionboxnumber{\thesubsection}}{3mm}{}[]

%==== MINTED SETTING ====%

\renewcommand{\theFancyVerbLine}{
    \sffamily
    \textcolor[rgb]{0.5,0.5,0.5}{
    \scriptsize{
\arabic{FancyVerbLine}}}}
\setminted{frame=single, linenos, fontsize=\normalsize}