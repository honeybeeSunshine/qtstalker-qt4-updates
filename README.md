qtstalker-qt4
=============
files from http://anu.homelinux.net/pub/Linux/ports/distfiles/qtstalker-qt4-branch.tar.bz2
by bdheeman

------------------

Licence found in docs/COPYING

The usual caveats apply in regards to me not being responsible in any way for your trading escapades or any bugs in this software.


Updates
============

Some additions to the qt4 version of Qtstalker...

- many more timeframes/barlengths, from 5 seconds up to Quarterly.
- filewatcher toggle to monitor updates to the currently open price database (realtime trading)
- additional and modified chart objects (speedlines, fibonacci improvements)
- option to open a chart directly from the command line
- modified key bindings to improve usability
- python helper and demo scripts to programmatically write and read to and from the databases (think price updates, ML labelling, etc.)


Installation
===========

Qt4 is getting on in years, and you may find the dev packages are no longer in your repository.
Sometime in 2022 I used the files here to build and install qtstalker:
https://launchpad.net/~gezakovacs/+archive/ubuntu/ppa/

You will also need TA-lib, and BerkeleyDB.

I used Qt 4.8.7 and libdb5.3.28-13.


Usage
===========

You can still import data using the CSV plugin or instead write directly to an appropriate database file. An example script that does the latter is given in /python/av_direct.py which fetches data from AlphaVantage. You will need a free api key. It can be easily adapted it to your data source, such as yfinance, your favouring crypto exchange, or interactive brokers for example.

I'm not recommending you use IB as a broker in any way whatsoever, but using the filewatcher feature, you can auto update the chart as "real time" 5 second bars stream in costing maybe $1.50 a month for all Nasdaq or NYSE.


Filewatcher
-----------
Toggle the 'eye' icon on the timeframe toolbar to redraw when this file updates


Barlengths
-----------
More barlengths are available from the drop-down.
'Custom' will show a Quarterly chart.
If trading crypto or something that trades on weekends, be sure to use, for example, '2Dw' which includes weekends, not '2D' which skips weekends, for a two-day bar.


Command Line
-----------

```
qtstalker
qtstalker filename
qtstalker filename barlength
```

It is assumed that .qtstalker is in your home directory and your data is located in {HOME}/.qtstalker/data1/data/Stocks/

You can optionally give qtstalker a filename to open on startup.
If you provide a filename, you can provide a barlength as an integer from 0 to 26 or so. The integer corresponds to the position in the barlength drop-down.


Chart Objects
----------
Angles and fibonacci ranges autoselect highs and lows by default when drawing.

Fibonacci changes colour for support/resistance on drawing

Helper functions to interact with the chart database directly can be found in /python/qts_cholib.py


Key Bindings
----------
You need to have draw mode toggled on, and cursor mode not on to actually draw

- ctrl+f : start drawing fibonacci range
- ctrl+a : start drawing speed line/angles
- ctrl+h : draw horizontal line
- ctrl+v : draw vertical line
- ctrl+b : label using buy arrows
- ctrl+s : label using sell arrows
- ctrl+z : remove chart selected object
- ctrl+w : change colour of selected chart object


Other Notes
=========
Qtstalker is a great piece of charting software, with great custom indicator capabilities and a scanning feature. It has some ui quirks, possible design limitations, and a few bugs. Just posting this in case anyone has a use for it. Feel free to send an email. Enjoy!
