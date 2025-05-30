#include "userapi/image_handler.hpp"
#include "liblvgl/core/lv_disp.h"
#include "liblvgl/core/lv_obj_tree.h"
#include "liblvgl/widgets/lv_img.h"
#include "pros/rtos.hpp"

namespace images {
    ImageHandler::ImageHandler(int default_delay) {
        this->delay = default_delay;
    }

    void ImageHandler::register_image(ImageEntry image) {
        this->images.push_back(image);
    }

    void ImageHandler::register_image(lv_img_dsc_t image) {
        this->images.push_back({image});
    }

    void ImageHandler::register_image(lv_img_dsc_t image, int delay) {
        this->images.push_back({image, delay});
    }

    void ImageHandler::register_images(std::vector<ImageEntry> images) {
        for (ImageEntry image : images) {
            this->images.push_back(image);
        }
    }

    void ImageHandler::set_default_delay(int delay) {
        this->delay = delay;
    }

    int ImageHandler::get_default_delay() {
        return this->delay;
    }

    void ImageHandler::remove() {
        this->stop();
        lv_obj_del(this->img);
    }

    void ImageHandler::start() {
        this->activated = true;
        this->img = lv_img_create(lv_scr_act());

        pros::Task render_images([&]{
            while (activated) {
                lv_img_set_src(this->img, &images[index].image);
                lv_obj_align(this->img, LV_ALIGN_CENTER, 0, 0);
                
                if (images[index].delay.has_value()) {
                    pros::delay(images[index].delay.value());
                } else {
                    pros::delay(this->delay);
                }
                index = (index + 1) % this->images.size();
            }
        });
    }

    void ImageHandler::stop() {
        this->activated = false;
    }
}