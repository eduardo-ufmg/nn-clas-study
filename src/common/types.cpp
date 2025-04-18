#include "types.hpp"

#include <cmath>
#include <algorithm>
#include <numeric>

using namespace std;

BaseSample::BaseSample(const SampleID id, const Coordinates& coordinates)
  : id(id), coordinates(coordinates)
{}

Sample::Sample(const SampleID id, const Coordinates& coordinates, shared_ptr<Cluster> cluster)
  : BaseSample(id, coordinates), cluster(cluster), quality(0.0f)
{}

Cluster::Cluster(const Target id)
  : id(id), sumq(0.0f), magq(0), online_avgq(0.0f), sumDeltaSq(0.0f), online_stdq(0.0f), threshold(0.0f)
{}

void Cluster::reset()
{
  sumq = 0.0f;
  magq = 0;
  online_avgq = 0.0f;
  sumDeltaSq = 0.0f;
  online_stdq = 0.0f;
  threshold = 0.0f;
}

void Cluster::accumQ_updateStats(const float q)
{
  sumq += q;
  ++ magq;

  const float delta = q - online_avgq;

  online_avgq += delta / magq;

  const float delta2 = delta - online_avgq;

  sumDeltaSq += delta * delta2;

  if (magq > 1) {
    online_stdq = sqrt(sumDeltaSq / (magq - 1));
  }
}

void Cluster::computeThreshold(const float tolerance)
{
  threshold = online_avgq - tolerance * online_stdq;
}

SupportSample::SupportSample(const SampleID id, const Coordinates& coordinates, const Target target)
  : BaseSample(id, coordinates), target(target)
{}

TestSample::TestSample(const SampleID id, const Coordinates& coordinates, const Target expectedtarget)
  : BaseSample(id, coordinates), expectedtarget(expectedtarget)
{}

PredictedSample::PredictedSample(const SampleID id, const Coordinates coordinates, const Target target)
  : BaseSample(id, coordinates), target(target)
{}
