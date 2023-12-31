#include <iostream>
#include <random>
#include <string>
#include <vector>

#include <math.h>

#include <Eigen/Dense>

#include "json.hpp"
#include "lib.hpp"

using json = nlohmann::json;

using real = double;

namespace nlohmann
{
template<>
struct adl_serializer<Eigen::Vector3d>
{
  static void to_json(json& j, const Eigen::Vector3d& vec)
  {
    j = json::array({vec[0], vec[1], vec[2]});
  }

  static void from_json(const json& j, Eigen::Vector3d& vec)
  {
    assert(j.size() == 3);
    vec[0] = j[0];
    vec[1] = j[1];
    vec[2] = j[2];
  }
};
}  // namespace nlohmann

struct boid
{
  Eigen::Vector3d pos;
  Eigen::Vector3d vel;

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(boid, pos, vel);
};

using boids = std::vector<boid>;

unsigned const num_boids = 2;

real const jitter_radius = 10;

real const flee_radius = 0.5;
real const flee_strength = 0.2;

real const flock_radius = 5;
real const flock_pos_strength = 0.1;
real const flock_vel_strength = 0.1;

real const speed_min = 0.5;
real const speed_max = 5;
real const bounds = 100;

bool is_one = true;
typename ::boids boids1;
typename ::boids boids2;

boids init()
{
  std::mt19937_64 twister;
  std::uniform_real_distribution<real> jitter {-jitter_radius, +jitter_radius};

  boids boids(num_boids);
  for (int i = 0; i < num_boids; i++) {
    boids.push_back(
        {{jitter(twister), jitter(twister), jitter(twister)},
         {jitter(twister) / 2, jitter(twister) / 2, jitter(twister) / 2}});
  }
  return boids;
}

void step()
{
  boids& boids = is_one ? boids1 : boids2;
  typename ::boids& update = is_one ? boids2 : boids1;

  update = boids;

  for (int i = 0; i < boids.size(); i++) {
    Eigen::Vector3d avg_vel;
    Eigen::Vector3d avg_pos;
    unsigned avg_count = 0;

    for (int j = 0; j < boids.size(); j++) {
      if (i == j) {
        continue;
      }
      const auto& boid = boids.at(i);
      const auto& other = boids.at(j);
      const auto diff = (boid.pos - other.pos);
      const real squared_dist = diff.squaredNorm();
      if (squared_dist < flee_radius * flee_radius) {
        update.at(i).vel += diff * flee_strength;
      }

      if (squared_dist < flock_radius * flock_radius) {
        avg_vel += other.vel;
        avg_pos += other.pos;
        avg_count++;
      }

      if (avg_count != 0) {
        update.at(i).vel +=
            (avg_vel - boid.vel) / avg_count * flock_vel_strength;
        update.at(i).vel +=
            (avg_pos - boid.pos) / avg_count * flock_pos_strength;
      }

      const real speed2 = update.at(i).vel.squaredNorm();
      if (speed2 != 0) {
        if (speed2 < speed_min * speed_min) {
          assert(speed2 != 0);
          update.at(i).vel = update.at(i).vel / sqrt(speed2) * speed_min;
        } else if (speed2 > speed_max * speed_max) {
          assert(speed2 != 0);
          update.at(i).vel = update.at(i).vel / sqrt(speed2) * speed_max;
        }
      }
    }
  }
  is_one ^= 1U;
}

void dump()
{
  std::cout << json {boids1} << std::endl;
}

int main()
{
  boids1 = init();
  dump();
  for (int i = 0; i < 10; i++) {
    step();
    dump();
  }
  return 0;
}
