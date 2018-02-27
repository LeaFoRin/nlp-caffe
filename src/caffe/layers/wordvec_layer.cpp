#include <vector>
#include "caffe/filler.hpp"
#include "caffe/util/math_functions.hpp"
#include "caffe/layers/wordvec_layer.hpp"

namespace caffe {

template <typename Dtype>
void WordvecLayer<Dtype>::LayerSetUp(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top) {
  WordvecParameter wordvec_param = this->layer_param_.wordvec_param();
  CHECK((wordvec_param.has_dimension()))
      << "wordvec_param.has_dimension()";
  CHECK((wordvec_param.has_vocab_size()))
      << "wordvec_param.has_vocab_size()";

  num_ = bottom[0]->num();
  dimension_ = wordvec_param.dimension();
  vocab_size_ = wordvec_param.vocab_size();
  sentence_length_ = bottom[0]->channels();

  this->blobs_.resize(1);
    this->blobs_[0].reset(new Blob<Dtype>(
        1, vocab_size_, 1, dimension_));

  shared_ptr<Filler<Dtype> > weight_filler(GetFiller<Dtype>(
      wordvec_param.weight_filler()));
  weight_filler->Fill(this->blobs_[0].get());

  // Propagate gradients to the parameters (as directed by backward pass).
  this->param_propagate_down_.resize(this->blobs_.size(), false);
}

template <typename Dtype>
void WordvecLayer<Dtype>::Reshape(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top) {
  CHECK((this->layer_param_.bottom_size() == 1 || this->layer_param_.bottom_size() == 0))
      << "Wordvec must have no more than one bottom";
  CHECK((this->layer_param_.top_size() == 1 || this->layer_param_.top_size() == 0))
      << "Wordvec must have no more than one top";
  top[0]->Reshape(num_, dimension_, sentence_length_, 1);
}

template <typename Dtype>
void WordvecLayer<Dtype>::Forward_cpu(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top) {
  const Dtype* weights = this->blobs_[0]->cpu_data();
  Dtype* bottom_data = bottom[0]->mutable_cpu_data();
  Dtype* top_data = top[0]->mutable_cpu_data();

  for (int n = 0; n < num_; ++n) {
    for (int d = 0; d < dimension_; ++d) {
      for (int i = 0; i < sentence_length_; ++i) {
        const int word = static_cast<int>(bottom_data[i + n * sentence_length_]
            + Dtype(0.5));
        top_data[i + (d + (n * dimension_)) * sentence_length_] = weights[d + word * dimension_];
      }
    }
  }
}

template <typename Dtype>
void WordvecLayer<Dtype>::Backward_cpu(const vector<Blob<Dtype>*>& top,
      const vector<bool>& propagate_down, const vector<Blob<Dtype>*>& bottom) {
  Dtype* weights_diff = this->blobs_[0]->mutable_cpu_diff();
  const Dtype* bottom_data = bottom[0]->cpu_data();
  const Dtype* top_diff = top[0]->cpu_diff();

  for (int n = 0; n < num_; ++n) {
    for (int d = 0; d < dimension_; ++d) {
      for (int i = 0; i < sentence_length_; ++i) {
        const int word = static_cast<int>(bottom_data[i + n * sentence_length_]
            + Dtype(0.5));
        weights_diff[d + word * dimension_] +=
            top_diff[i + (d + (n * dimension_)) * sentence_length_];
      }
    }
  }
}

#ifdef CPU_ONLY
STUB_GPU(WordvecLayer);
#endif

INSTANTIATE_CLASS(WordvecLayer);
REGISTER_LAYER_CLASS(Wordvec);

}  // namespace caffe
