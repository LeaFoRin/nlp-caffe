# NLP-Caffe

NLP-Caffe is a <a href="https://github.com/Russell91/nlp_caffe/blob/master/CHANGES.txt" target="_blank">pull request</a> [1] on the Caffe framework developed by Yangqing Jia and Evan Shelhamer, among other members of the BVLC lab at Berkeley and a large number of independent online contributers. 

This fork makes it easier for NLP users to get started without merging C++ code. The current example constructs a language model for a small subset of Google's Billion Word corpus. It uses a two-layer LSTM architecture that processes in excess of 15,000 words per second [2], and achieves a perplexity of 79. More examples for Machine Translation using the encoder-decoder model and character-level RNNs are in the works. This code will eventually be merged into the Caffe master branch. This work was funded by the <a href="http://nlp.stanford.edu/" target="_blank">Stanford NLP Group</a>, under the guidance of <a href="http://nlp.stanford.edu/~manning/" target="_blank">Chris Manning</a>, and with the invaluable expertise of <a href="http://stanford.edu/~lmthang/" target="_blank">Thang Luong</a>.

# Installation

We recommend consulting the Caffe <a href="http://caffe.berkeleyvision.org/installation.html" target="_blank">installation instructions</a> and compiling the standard Caffe library first. Next, clone this repo and run `make -j8 && make pycaffe` from the NLP-Caffe folder.

NLP-Caffe also requires <a href="https://github.com/dw/py-lmdb/" target="_blank">py-lmdb</a> at runtime, which can be installed with:

    pip install lmdb

# Example

First, cd to the caffe root directory and download the data for the model with:

    ./data/language_model/get_lm.sh

Using this data, you can generate the LMDB databases and the architecture train_val.prototxt with:

    python ./examples/language_model/create_lm.py --make_data

You'll notice this generates train, test, and validation databases in examples/language_model. It also generates the train_val.prototxt architecture file and the solver.prototxt hyperparameter file. By editing this file, you can control the hyperparameters, dataset, and architecture used by NLP-Caffe with a python interface.

You can now begin to train the network with:

    ./examples/language_model/train_lm.sh

The resulting wordvectors can be viewed with:

    ipython notebook ./examples/language_model/lm_visualization.ipynb

# Further reading

To get a better general understanding of how Caffe works, you can take advantage of the content in the <a href="http://caffe.berkeleyvision.org/installation.html" target="_blank">caffe tutorials section</a>. In particular, the <a href="http://nbviewer.ipython.org/github/BVLC/caffe/blob/master/examples/filter_visualization.ipynb" target="_blank">filter visualization tutorial</a> is a great example of how you can use the IPython notebook to investigate the weights and performance of a trained model. The <a href="http://caffe.berkeleyvision.org/gathered/examples/mnist.html" target="_blank">MNIST tutorial</a> is useful to understand how you can control Caffe over the command line and through architecture description text files.

# What's changed?

Caffe's current shared parameter implementation is pretty basic and duplicates the weight diff for each layer sharing the parameter. The net.cpp and solver.cpp files have been modified to remove this restriction and allow a constant overhead for sharing parameters that is only 50% more than the cost of a singly owned parameter, no matter how many layers are involved. Given this modification, you can build an RNN in caffe just by unrolling the number of layers you want in the prototxt file. Of course, you won't want to do this by hand, so we use the python google protobuf library to generate the prototxt. This has nice advantages like letting you create the train_val.prototxt and deploy.prototxt files simultaneously, generate random hyperparameters for multiple runs, or kick off 32 jobs at once on a cluster in just a few lines of python.

There are a few limitations of the implementation to be aware of. First, the library assumes each sentence is at most N words long, and that you have zero padded shorter sentences. The zero padding does not contribute to the loss function, but still costs processing time. There are a few ways this could be fixed, but none have been implemented yet. Fortunately, you can train on nets of a fixed max length and test on nets of a longer length. Constructing nets with a new lstm layer for each word in the sentence can also be a little daunting at first, but it's actually quite easy to manipulate all of these layers with python for loops once you get used to the idea.

An lstm layer and wordvec layer have been added, which are crucial for the language model. The slice layer has been modified to support a much faster special use case. The concat layer has received a significant speed up by taking advantage of the cublas library to perform a matrix transpose before copying memory. This allows memory to be copied in 100x bigger chunks, reducing layer forward/backward times by more than 10x when concatenating along the channel dimension. Specific changes in the code can be seen in the CHANGES.txt file.

As general advice, modifying this code to approach an entirely new NLP problem in Caffe is not for the feint of heart. On the other hand, if your research involves language models or similar problems (e.g. Part of Speech tagging), and you would like to experiment with more fancy architectures (e.g. convnet/lstm combos or clockwork lstms), then this codebase would be an excellent place to get started. You will have a very fast base implementation and all the modular goodness that comes with Caffe.

<br>

[1] All citations should be addressed to the <a href="https://github.com/BVLC/caffe" target="_blank">main Caffe repository</a>. Licensing is identical to that of a Caffe pull request.

[2] The average sentence has 22.2 words when long sentences are capped to 30 words. We train batches of 128 sentences with an average batch time of 0.166 s on a Nvidia GTX 780 TI.
