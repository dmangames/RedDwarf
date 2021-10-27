#include "CollisionManager.h"

CollisionManager::CollisionManager(std::vector<GameActor*>* actors) {
	this->actors = actors;
}


// Returns a scalar value that represents the position of a given point as
// projected along a given axis
float project_point(SDL_Point point, SDL_Point axis) {

	float multiplier = (point.x * axis.x + point.y * axis.y) /
		(pow(axis.x, 2) + pow(axis.y, 2));
	return multiplier * pow(axis.x, 2) + multiplier * pow(axis.y, 2);

}

// Determine minimum and maxiumum scalar values from all corners
// on a given axis
void set_min_max(Hitbox* h, SDL_Point axis, float* min, float* max) {

	float smallest, biggest;

	smallest = biggest = project_point(h->get_tl(), axis);

	float n = project_point(h->get_tr(), axis);
	if (n > biggest) { biggest = n; }
	if (n < smallest) { smallest = n; }

	n = project_point(h->get_bl(), axis);
	if (n > biggest) { biggest = n; }
	if (n < smallest) { smallest = n; }

	n = project_point(h->get_br(), axis);
	if (n > biggest) { biggest = n; }
	if (n < smallest) { smallest = n; }

	*min = smallest;
	*max = biggest;

}

// Return true if two given hitboxes are overlapping as
// projected on a given axis
bool test_axis(Hitbox* h1, Hitbox* h2, SDL_Point axis) {

	float h1_min, h1_max, h2_min, h2_max;

	set_min_max(h1, axis, &h1_min, &h1_max);
	set_min_max(h2, axis, &h2_min, &h2_max);

	return (h2_min <= h1_max && h2_min >= h1_min) || (h2_max <= h1_max && h2_max >= h1_min);

}

//// Returns true if two hitboxes are overlapping
//bool check_hitboxes(Hitbox* h1, Hitbox* h2) {
//
//	// Check by radius method to avoid expensive Separating Axis Method when possible
//	// Calculate distance of each center
//	float center_distance = pow(h1->get_center_x() - h2->get_center_x(), 2.0f) + pow(h1->get_center_y() - h2->get_center_y(), 2.0f);
//
//	// Add radii from both hitboxes
//	float total_radii = h1->get_radius() + h2->get_radius();
//	if (total_radii > center_distance) {
//
//		// Radius method is inconclusive, measure with Separating Axis Theorem instead
//		// https://www.gamedev.net/resources/_/technical/game-programming/2d-rotated-rectangle-collision-r2604
//		SDL_Point axis1, axis2, axis3, axis4;
//
//		axis1.x = h1->get_tr().x - h1->get_tl().x;
//		axis1.y = h1->get_tr().y - h1->get_tl().y;
//		if (!test_axis(h1, h2, axis1)) { return false; }
//
//
//		axis2.x = h1->get_tr().x - h1->get_br().x;
//		axis2.y = h1->get_tr().y - h1->get_br().y;
//		if (!test_axis(h1, h2, axis2)) { return false; }
//
//		axis3.x = h2->get_tl().x - h2->get_bl().x;
//		axis3.y = h2->get_tl().y - h2->get_bl().y;
//		if (!test_axis(h1, h2, axis3)) { return false; }
//
//		axis4.x = h2->get_tl().x - h2->get_tr().x;
//		axis4.y = h2->get_tl().y - h2->get_tr().y;
//		if (!test_axis(h1, h2, axis4)) { return false; }
//
//		return true;
//	}
//
//	return false;
//
//}

// Returns true if two hitboxes are overlapping
bool check_hitboxes(Hitbox* h1, Hitbox* h2) {

	// Check by radius method to avoid expensive Separating Axis Method when possible
	// Calculate distance of each center
	float center_distance = pow(h1->get_center_x() - h2->get_center_x(), 2.0f) + pow(h1->get_center_y() - h2->get_center_y(), 2.0f);

	// Add radii from both hitboxes
	float total_radii = h1->get_radius() + h2->get_radius();
	if (total_radii > center_distance) {
		
		// Simple rect intesect with no rotation
		// https://gamedev.stackexchange.com/questions/586/what-is-the-fastest-way-to-work-out-2d-bounding-box-intersection
		return !(h2->get_tl().x > h1->get_tr().x
			|| h2->get_tr().x < h1->get_tl().x
			|| h2->get_tl().y > h1->get_bl().y
			|| h2->get_bl().y < h1->get_tl().y);
	}

	return false;

}

// Check for collisions and notify entities
void CollisionManager::check_collisions() {

	for (int i = 0; i < actors->size(); i++) {
		if ((*actors)[i]->collides() && (*actors)[i]->is_active()) {
			for (int j = 0; j < actors->size(); j++) {
				// Make sure not to check it against itself
				if (j != i) {
					if ((*actors)[i]->does_collide((*actors)[j]->get_id())) {
						// Do the hitboxes overlap?
						if (check_hitboxes((*actors)[i]->get_hitbox(), (*actors)[j]->get_hitbox())) {
							// If so, notify the entites if both entities are active
							if ((*actors)[i]->is_active() && (*actors)[j]->is_active()) {
								(*actors)[i]->collide_actor((*actors)[j]);
								(*actors)[j]->collide_actor((*actors)[i]);
							}
							
						}
					}
				}
			}
		}
	}

}