
#include "Raycaster.h"

void Raycaster::LoadTextures() {

    sf::Texture hud_textures;
    hud_textures.loadFromFile("../assets/hud.png");

    sf::Texture object_textures;
    hud_textures.loadFromFile("../assets/objects.png");

    sf::Image wall_textures;
    hud_textures.loadFromFile("../assets/walls.png");

    sf::Texture weapon_textures;
    hud_textures.loadFromFile("../assets/weapons.png");

    tile_map.push_back(wall_textures);

}

void Raycaster::Cast() {

}

Raycaster::Raycaster(std::shared_ptr<Map> map, std::shared_ptr<Camera> camera) :
        map(map), camera(camera) {
	
	for (int i = 0; i < 8; i++)
		thread_pool.emplace_back(std::thread(&Raycaster::MarchThread, this));
}

Raycaster::~Raycaster(){
	end_state = false;

	for (auto &&i : thread_pool) {
		i.join();
	}
}

void Raycaster::CreateViewport(sf::Vector2i resolution, sf::Vector2f fov)
{
    // CL needs the screen resolution
    viewport_resolution = resolution;

    // And an array of vectors describing the way the "lens" of our
    // camera works
    // This could be modified to make some odd looking camera lenses

    double y_increment_radians = DegreesToRadians(fov.y / viewport_resolution.y);
    double x_increment_radians = DegreesToRadians(fov.x / viewport_resolution.x);

    viewport_matrix = new sf::Vector4f[resolution.x * resolution.y * 4];

    for (int y = -viewport_resolution.y / 2, y_p = 0; y < viewport_resolution.y / 2; y++, y_p++) {
        for (int x = -viewport_resolution.x / 2, y_x = 0; x < viewport_resolution.x / 2; x++, y_x++) {

            // The base ray direction to slew from
            sf::Vector3f ray(1, 0, 0);

            // Y axis, pitch
            ray = sf::Vector3f(
                    static_cast<float>(ray.z * sin(y_increment_radians * y) + ray.x * cos(y_increment_radians * y)),
                    static_cast<float>(ray.y),
                    static_cast<float>(ray.z * cos(y_increment_radians * y) - ray.x * sin(y_increment_radians * y))
            );


            // Z axis, yaw
            ray = sf::Vector3f(
                    static_cast<float>(ray.x * cos(x_increment_radians * x) - ray.y * sin(x_increment_radians * x)),
                    static_cast<float>(ray.x * sin(x_increment_radians * x) + ray.y * cos(x_increment_radians * x)),
                    static_cast<float>(ray.z)
            );

            // correct for the base ray pointing to (1, 0, 0) as (0, 0). Should equal (1.57, 0)
            ray = sf::Vector3f(
                    static_cast<float>(ray.z * sin(-1.57) + ray.x * cos(-1.57)),
                    static_cast<float>(ray.y),
                    static_cast<float>(ray.z * cos(-1.57) - ray.x * sin(-1.57))
            );

            int index = y_p * viewport_resolution.x + y_x;
            ray = Normalize(ray);

            viewport_matrix[index] = sf::Vector4f(
                    ray.x,
                    ray.y,
                    ray.z,
                    0
            );
        }
    }

    // Create the image that opencl's rays write to
    viewport_image = new sf::Uint8[resolution.x * resolution.y * 4];

    for (int i = 0; i < resolution.x * resolution.y * 4; i += 4) {

        viewport_image[i] = 255;     // R
        viewport_image[i + 1] = 255; // G
        viewport_image[i + 2] = 255; // B
        viewport_image[i + 3] = 255; // A
    }

    viewport_texture.create(resolution.x, resolution.y);
    viewport_texture.update(viewport_image);
    viewport_sprite.setTexture(viewport_texture);
	viewport_sprite.setScale(2.0, 2.0);

}



void Raycaster::Draw(sf::RenderWindow * window) {
    viewport_texture.update(viewport_image);
    window->draw(viewport_sprite);
}

void Raycaster::March() {

    int thread_count = 40;
    for (int i = 0; i < thread_count; i++) {
        sf::Vector2i start = sf::Vector2i(0, ((viewport_resolution.y / thread_count) * i));
        sf::Vector2i end = sf::Vector2i(viewport_resolution.x, ((viewport_resolution.y / thread_count) * (i + 1)));
		queue_mutex.lock();
        block_queue.push(std::make_pair(start, end));
		queue_mutex.unlock();
    }

	while (!block_queue.empty()) {
		
	}

}

void Raycaster::MarchThread() {

	while (!end_state) {

		queue_mutex.lock();

		if (!block_queue.empty()) {
			std::pair<sf::Vector2i, sf::Vector2i> block = block_queue.front();
			block_queue.pop();
			queue_mutex.unlock();
			MarchSingle(block.first, block.second);
		}
		else {
			queue_mutex.unlock();
			using namespace std::chrono_literals;
			std::this_thread::sleep_for(1ms);
		}
	}
    
}

void Raycaster::MarchSingle(sf::Vector2i start, sf::Vector2i end) {

    sf::Vector2i pixel = start;
    F_Vec4f ray_dir;
    sf::Vector3i voxel_step;
    sf::Vector3i voxel;
    sf::Vector3f delta_t;
	
    sf::Vector3f intersection_t;
    sf::Vector3i mask;
    sf::Vector3i overshoot;
    sf::Vector3i undershoot;

	sf::Vector2f cam_dir_polar = camera->getDirectionPolar();
	sf::Vector3f cam_pos = camera->getPosition();
	sf::Vector3i bound = map->getDimensions();

	// offset is how far we are into a voxel, enables sub voxel movement
	sf::Vector3f offset = sf::Vector3f(
		-(cam_pos.x - floor(cam_pos.x)) * voxel_step.x,
		-(cam_pos.y - floor(cam_pos.y)) * voxel_step.y,
		-(cam_pos.z - floor(cam_pos.z)) * voxel_step.z
	);
    
	while (pixel.x < end.x){
        while (pixel.y < end.y){

            // 4f 3f ??
            ray_dir = viewport_matrix[pixel.x + viewport_resolution.x * pixel.y];


			float cam_x_sin = sin(cam_dir_polar.x);
			float cam_x_cos = cos(cam_dir_polar.x);
            
			float cam_y_sin = sin(cam_dir_polar.y);
			float cam_y_cos = cos(cam_dir_polar.y);

			ray_dir.noCopy(
				ray_dir.z * cam_x_sin + ray_dir.x * cam_x_cos,
				ray_dir.y,
				ray_dir.z * cam_x_cos - ray_dir.x * cam_x_sin,
				0
			);

			ray_dir.noCopy(
				ray_dir.x * cam_y_cos - ray_dir.y * cam_y_sin,
				ray_dir.x * cam_y_sin + ray_dir.y * cam_y_cos,
				ray_dir.z,
				0
			);
            
			
            // Setup the voxel step based on what direction the ray is pointing
            voxel_step.x = static_cast<int>((ray_dir.x > 0) - (ray_dir.x < 0));
			voxel_step.y = static_cast<int>((ray_dir.y > 0) - (ray_dir.y < 0));
			voxel_step.z = static_cast<int>((ray_dir.z > 0) - (ray_dir.z < 0));
            

            // Setup the voxel coords from the camera origin
			voxel.x = static_cast<int>(cam_pos.x);
			voxel.y = static_cast<int>(cam_pos.y);
			voxel.z = static_cast<int>(cam_pos.z);
			

            // Delta T is the units a ray must travel along an axis in order to
            // traverse an integer split
			delta_t.x = fabs(1.0f / ray_dir.x);
			delta_t.y = fabs(1.0f / ray_dir.y);
			delta_t.z = fabs(1.0f / ray_dir.z);
            

            // Intersection T is the collection of the next intersection points
            // for all 3 axis XYZ.
            intersection_t = sf::Vector3f(
                    delta_t.x * offset.x,
                    delta_t.y * offset.y,
                    delta_t.z * offset.z
            );

            // for negative values, wrap around the delta_t, rather not do this
            // component wise, but it doesn't appear to want to work
            if (intersection_t.x < 0) {
                intersection_t.x += delta_t.x ;//* -std::isless(intersection_t.x, 0);
            }
            if (intersection_t.y < 0) {
                intersection_t.y += delta_t.y;// * -std::isless(intersection_t.y, 0);;
            }
            if (intersection_t.z < 0) {
                intersection_t.z += delta_t.z;// * -std::isless(intersection_t.z, 0);;
            }


            int max_dist = 800;
            int dist = 0;

            mask = { 0, 0, 0 };

            // Andrew Woo's raycasting algo
            do {

                if ((intersection_t.x) < (intersection_t.y)) {
                    if ((intersection_t.x) < (intersection_t.z)) {

                        mask.x = 1;
                        voxel.x += voxel_step.x;
                        intersection_t.x = intersection_t.x + delta_t.x;
                    }
                    else {

                        mask.z = 1;
                        voxel.z += voxel_step.z;
                        intersection_t.z = intersection_t.z + delta_t.z;
                    }
                }
                else {
                    if ((intersection_t.y) < (intersection_t.z)) {

                        mask.y = 1;
                        voxel.y += voxel_step.y;
                        intersection_t.y = intersection_t.y + delta_t.y;
                    }
                    else {

                        mask.z = 1;
                        voxel.z += voxel_step.z;
                        intersection_t.z = intersection_t.z + delta_t.z;
                    }
                }

                // If the ray went out of bounds
				overshoot.x = voxel.x < bound.x;
				overshoot.y = voxel.y < bound.y;
				overshoot.z = voxel.z < bound.z;


                undershoot.x = voxel.x >= 0;
				undershoot.y = voxel.y >= 0;
				undershoot.z = voxel.z >= 0;


                if (overshoot.x == 0 || overshoot.y == 0 || overshoot.z == 0 || undershoot.x == 0 || undershoot.y == 0) {
                    Blit(sf::Color::Blue, sf::Vector2i{ pixel.x,pixel.y }, mask);
                    break;
                }
                if (undershoot.z == 0) {
                    Blit(sf::Color::Red, sf::Vector2i{ pixel.x,pixel.y }, mask);
                    break;
                }

                int voxel_data = map.get()->getGrid(voxel);

                if (voxel_data != 0) {
                    switch (voxel_data) {
                        case 1:
                            Blit(sf::Color::Green, sf::Vector2i{ pixel.x,pixel.y }, mask);
                            break;
                        case 2:
                            Blit(sf::Color::Green, sf::Vector2i{ pixel.x,pixel.y }, mask);
                            break;
                        case 3:
                            Blit(sf::Color::Green, sf::Vector2i{ pixel.x,pixel.y }, mask);
                            break;
                        case 4:
                            Blit(sf::Color::Green, sf::Vector2i{ pixel.x,pixel.y }, mask);
                            break;
                        case 5:
                            Blit(sf::Color(30, 10, 200, 100), sf::Vector2i{ pixel.x,pixel.y }, mask);
                            break;
                        case 6:
                            Blit(sf::Color::Green, sf::Vector2i{ pixel.x,pixel.y }, mask);
                            break;
                        default:
                            //write_imagef(image, pixel, (float4)(.30, .2550, .2550, 255.00));
                            break;
                    }
                    break;
                }

                dist++;
            } while (dist < max_dist);

            //Blit(sf::Color::Red, sf::Vector2i{ pixel.x,pixel.y }, mask);
            pixel.y++;
            continue;

        }
        ++pixel.x;
        pixel.y = start.y;
    }
}

void Raycaster::Blit(sf::Color color, sf::Vector2i position, sf::Vector3i mask) {

	memcpy(&viewport_image[(position.x + viewport_resolution.x * position.y) * 4 + 0], &color, sizeof(sf::Color));
   /* viewport_image[(position.x + viewport_resolution.x * position.y) * 4 + 0] = color.r;
    viewport_image[(position.x + viewport_resolution.x * position.y) * 4 + 1] = color.g;
    viewport_image[(position.x + viewport_resolution.x * position.y) * 4 + 2] = color.b;
    viewport_image[(position.x + viewport_resolution.x * position.y) * 4 + 3] = color.a;*/
}
