#include "writeFiles.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "types.hpp"
#include "classifier.pb.h"

using namespace std;

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

ofstream openFileWrite(const string& filename);

int writeSSs(const SupportSamples& supportSamples, const string& filename)
{
  classifierpb::SupportSamples pb_supportSamples;

  for (const SupportSample& sample : supportSamples) {
    classifierpb::SupportSampleEntry *pb_sample = pb_supportSamples.add_entries();
    
    pb_sample->set_sample_id(sample.id);

    for (const float coord : sample.coordinates) {
      pb_sample->add_features(coord);
    }

    classifierpb::Target * pb_target = make_unique<classifierpb::Target>().release();

    visit(overloaded {
      [pb_target](const int id) { pb_target->set_target_int(id); },
      [pb_target](const string& id) { pb_target->set_target_str(id); }
    }, sample.target);


    pb_sample->set_allocated_target(pb_target);
  }

  ofstream file = openFileWrite(filename);
  if (!pb_supportSamples.SerializeToOstream(&file)) {
    cerr << "Error: could not write SSs to file" << filename << endl;
    return 1;
  }
  file.close();

  return 0;
}

int writePredictedSamples(const PredictedSamples& predictedSamples, const string& filename)
{
  classifierpb::PredictedSamples pb_predictedSamples;

  for (const PredictedSample& sample : predictedSamples) {
    classifierpb::PredictedSampleEntry *pb_sample = pb_predictedSamples.add_entries();
    
    pb_sample->set_sample_id(sample.id);

    for (const float coord : sample.coordinates) {
      pb_sample->add_features(coord);
    }

    classifierpb::Target * pb_target = make_unique<classifierpb::Target>().release();

    visit(overloaded {
      [pb_target](const int id) { pb_target->set_target_int(id); },
      [pb_target](const string& id) { pb_target->set_target_str(id); }
    }, sample.target);

    pb_sample->set_allocated_target(pb_target);
  }

  ofstream file = openFileWrite(filename);
  if (!pb_predictedSamples.SerializeToOstream(&file)) {
    cerr << "Error: could not write predicted samples to file" << filename << endl;
    return 1;
  }
  file.close();

  return 0;
}

int writeLikelihoods(const PredictedSamples& predictedSamples, const std::string& filename)
{
  classifierpb::PredictedSamples pb_predictedSamples;

  for (const PredictedSample& sample : predictedSamples) {
    classifierpb::PredictedSampleEntry* pb_sample = pb_predictedSamples.add_entries();

    pb_sample->set_sample_id(sample.id);

    for (const float coord : sample.coordinates) {
      pb_sample->add_features(coord);
    }

    classifierpb::Likelihoods* pb_likelihoods = pb_sample->mutable_likelihoods();

    classifierpb::Likelihood* pb_likelihood0 = pb_likelihoods->mutable_likelihood0();
    pb_likelihood0->set_likelihood(sample.likelihoods.first.first);
    visit(overloaded{
      [pb_likelihood0](const int id) { pb_likelihood0->mutable_target()->set_target_int(id); },
      [pb_likelihood0](const std::string& id) { pb_likelihood0->mutable_target()->set_target_str(id); }
    }, sample.likelihoods.first.second);

    classifierpb::Likelihood* pb_likelihood1 = pb_likelihoods->mutable_likelihood1();
    pb_likelihood1->set_likelihood(sample.likelihoods.second.first);
    visit(overloaded{
      [pb_likelihood1](const int id) { pb_likelihood1->mutable_target()->set_target_int(id); },
      [pb_likelihood1](const std::string& id) { pb_likelihood1->mutable_target()->set_target_str(id); }
    }, sample.likelihoods.second.second);
  }

  ofstream file = openFileWrite(filename);
  if (!pb_predictedSamples.SerializeToOstream(&file)) {
    cerr << "Error: could not write likelihoods to file " << filename << endl;
    return 1;
  }
  file.close();

  return 0;
}

ofstream openFileWrite(const string& filename)
{
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ofstream file(filename, ios::binary);
  if (!file.is_open()) {
    throw runtime_error("Could not open file " + filename);
  }
  return file;
}
