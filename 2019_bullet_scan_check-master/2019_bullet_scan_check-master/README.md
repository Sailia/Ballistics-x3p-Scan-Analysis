# Bullet Scan Check

2019 REU Project - High-level checks of bullet scan quality

This summer, you'll be helping us to build a user interface to perform automatic quality checks of three-dimensional microscopy scan files. CSAFE, the Center for Statistical Applications in Forensic Evidence, is actively working to develop a method to automatically compare scans of bullets and cartridge cases; to do this, we need to make sure our scans conform to certain quality controls, which is where this project comes in.

## Initial Steps

#### GitHub

This summer, you'll be using GitHub to keep track of code and other work you perform. Please [create a github account](https://github.com/join) if you don't already have one, and send your username to Susan so you can be added to the CSAFE group. [Here](https://www.elegantthemes.com/blog/resources/git-and-github-a-beginners-guide-for-complete-newbies) is a guide on git and GitHub that may answer a few of your questions.


#### Background Reading

We understand you're still finishing up the semester, so all of this reading is optional. With that said, if you're excited for the summer and finished with your obligations for this semester, feel free to check out some of the following links:

- The Association of Firearm and Toolmark Examiners (AFTE) has a [web-based training](https://projects.nfstc.org/firearms/) for firearms examination. Not all parts of this are relevant: focus on the "anatomy" of a bullet, the features examiners look for when comparing a bullet, and a general overview of the terminology. Specifically, 
    - Chapter 2: [History](https://projects.nfstc.org/firearms/module02/fir_m02.htm) - a good historical perspective, with context for how the field developed. Not essential, but good trivia potential.
    - Chapter 4: Firearms Manufacture (specifically, [rifled barrels](https://projects.nfstc.org/firearms/module04/fir_m04_t06.htm)) - you don't need to know this whole thing, but the rifled barrel manufacturing process will be useful.
    - Chapter 11: [Bullet Comparison](https://projects.nfstc.org/firearms/module11/fir_m11.htm) - this is the part that most relates to what you're going to work with over the summer
    
- The data we have is in x3p files, which have a [specific format](https://www.openfmc.org/). We're going to ask you to develop ways to quickly check whether these files contain "good" data. There are C/C++ libraries for working with this file format, so you won't have to do anything from scratch. If you're familiar with C/C++, you might find the [github repository](https://github.com/OpenFMC/x3p) interesting.


#### Other Software:

- [R](https://cloud.r-project.org/) 

- [RStudio](https://www.rstudio.com/products/rstudio/download/) - RStudio is an Integrated Development Environment (like VS Code) for R. It's amazing, and if you're going to use R, you should use RStudio as well. 

- [x3ptools R package](https://github.com/heike/x3ptools) - install in R:

```r
install.packages("devtools")
library(devtools) # load the devtools package
install_github("heike/x3ptools") # install the x3ptools package from github
```

- [R cheat sheet](https://cloud.r-project.org/doc/contrib/refcard.pdf) - most of the useful functions in base R are on here - they'll get you started.

- [Stat 579 (Intro to R) Schedule and Slides from 2018](https://stat579-at-isu.github.io/schedule.html) (slides are linked, code from class is also provided). 

#### Other Information

- [Project documentation](https://csafe-isu.github.io/2019_bullet_scan_check)

- We have developed a google chrome extension to read and annotate x3p files called fix3p. It's in the Chrome Web Store - please go ahead and install it. 
    - The github repository can be found here: https://github.com/talenfisher/fix3p
    - The x3p.js javascript library is here: https://github.com/talenfisher/x3p.js

- This repository can be found at https://github.com/CSAFE-ISU/2019_bullet_scan_check

