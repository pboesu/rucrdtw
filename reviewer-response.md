> -  [x] A package-level help file (`?rucrdtw` or `?rucrdtw-package`) that gives an overall overview, links to the original UCR web page and reference and vignette.

added in a89f4e003c7cda844bd3fa0dd9cd082c8c068335

> -  [x] Function-level help files should have examples as well as `@value` fields documenting what the components of the list in function outputs are. This may resemble the "Output Explanation" of the UCR suite README.
-  [x] Argument explanations should be a little more detailed, in particular, greater explanation of the `qlength`, `dtwwindow`arguments. References to the UCR suite paper may be added to keep this from being too long.

I have expanded the documentation to include these elements (multiple commits up to fc6043cb)

> -  [x] Other useful information that could be in vignette, README, or package-level
  help file would be performance and functionality comparison to **dtw** or other similar R packages.
  
I have added a reference to the `dtw` package to the paper, the vignette, and the package-level helpfile. I have also added a benchmark comparing a naive sliding-window search with `dtw` to the `ucrdtw` method. (8618b05e)


> Another thing I think could be useful (for users who "first encounter the UCR suite via this package") are examples of practical application of the functionalities, ideally one for each of "classification, clustering, and anomaly detection", with or without reference to scientific articles.

rucrdtw is really only aimed at using DTW for similarity searches. I have rewritten the paper.md to highlight this, and added a reference and link to the `dtw` package. (commit ca31b2e)
I also appreciate the request for a real-world example. I've obviously put together the package in the first place because I was in need of a solution for a problem that arose in my research. However, at this point I am not able to publish that particular application. I have had a look for more applied examples, and have started playing with electrocardiogram data in [real-world-example](https://github.com/pboesu/rucrdtw/tree/real-world-example). But these data come with their own set of complexities and licensing issues (and I'm not an expert in human physiology). While I do see the added value this would bring, I would prefer to not spend time on making examples with an unfamiliar dataset work, unless you think this is absolutlely essential. I will certainly keep this in mind for future versions of the package.

> The functions accept different type of data (vector vs. matrix) and format (text files vs. R objects). It'd be great if the "overall overview" of the package-level documentation included a good guide on how to choose a function.

I have added a table with an overview of the different methods and input types into `?rucrdtw-package` (commit cd612f6)

> One particular point that I find misleading/hard to understand is why qlength isn't always length(query).

This is a design feature I retained from the original UCR Suite code, without much thought to it. I am guessing the original underlying idea was to leverage user knowledge to avoid reading the query file twice. However, I see now, how this may be confusing. 
I decided to remove it as a user input for the fv, vv, and mv methods, given that passing in the query as an R vector enables the user to decide on the query length before the function call. I did, however leave it as an argument in the ff method to avoid reading the query file twice, and to keep the API of that function identical to the command line version of the UCR Suite.



