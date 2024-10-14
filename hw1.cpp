#include "hw1.h"
#include "hw1_scenes.h"

using namespace hw1;

Image3 hw_1_1(const std::vector<std::string> &params) {
    // Homework 1.1: render a circle at the specified
    // position, with the specified radius and color.

    Image3 img(640 /* width */, 480 /* height */);

    Vector2 center = Vector2{img.width / 2 + Real(0.5), img.height / 2 + Real(0.5)};
    Real radius = 100.0;
    Vector3 color = Vector3{1.0, 0.5, 0.5};
    for (int i = 0; i < (int)params.size(); i++) {
        if (params[i] == "-center") {
            Real x = std::stof(params[++i]);
            Real y = std::stof(params[++i]);
            center = Vector2{x, y};
        } else if (params[i] == "-radius") {
            radius = std::stof(params[++i]);
        } else if (params[i] == "-color") {
            Real r = std::stof(params[++i]);
            Real g = std::stof(params[++i]);
            Real b = std::stof(params[++i]);
            color = Vector3{r, g, b};
        }
    }

    /*
    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            Vector3 bgColor = Vector3{0.5,0.5,0.5};
        }
    }
    */

    Vector3 bgColor = Vector3{0.5,0.5,0.5};
    
    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {

            Real pixelCenterX = x + 0.5;
            Real pixelCenterY = y + 0.5;

            Real distX = pixelCenterX - center.x;
            Real distY = pixelCenterY - center.y;

            Real length = std::sqrt(distX*distX + distY*distY);

            if (length <= radius) {
                img(x,y) = color;
            }
            else {
                img(x,y) = bgColor;
            }
        }
    }

    return img;
}

Image3 hw_1_2(const std::vector<std::string> &params) {
    // Homework 1.2: render multiple circles
    if (params.size() == 0) {
        return Image3(0, 0);
    }

    int scene_id = std::stoi(params[0]);
    const CircleScene &scene = hw1_2_scenes[scene_id];

    Image3 img(scene.resolution.x, scene.resolution.y);

    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            img(x, y) = Vector3{0.5,0.5,0.5};
        }
    }

    //for each circle, check all pixels
    for (Circle circle : scene.objects){
        Vector2 center = circle.center;
        Real radius = circle.radius;
        Vector3 color = circle.color;

        for (int y = 0; y < img.height; y++) {
            for (int x = 0; x < img.width; x++) {

                Real distanceX = x - center.x;
                Real distanceY = y - center.y;

                Real length = std::sqrt(distanceX * distanceX + distanceY * distanceY);

                if (length <= radius) {
                    img(x,y) = color;
                }
            }
        }
    }

    return img;
}

Image3 hw_1_3(const std::vector<std::string> &params) {
    // Homework 1.3: render multiple shapes
    if (params.size() == 0) {
        return Image3(0, 0);
    }

    Scene scene = parse_scene(params[0]);
    std::cout << scene << std::endl;

    Image3 img(scene.resolution.x, scene.resolution.y);

    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            img(x, y) = Vector3{0.5,0.5,0.5};
        }
    }

    for (Shape shape : scene.shapes) {
        if (auto *circle = std::get_if<Circle>(&shape)) {
            Vector2 center = circle->center;
            Real radius = circle->radius;
            Vector3 color = circle->color;

            for (int y = 0; y < img.height; y++){
                for (int x = 0; x < img.width; x++) {
                    Real distanceX = x - center.x;
                    Real distanceY = y - center.y;

                    Real length = std::sqrt(distanceX * distanceX + distanceY * distanceY);

                    if (length <= radius) {
                        img(x,y) = color;
                    }
                }
            }
        }
        else if (auto *rectangle = std::get_if<Rectangle>(&shape)){
            Vector2 topLeft = rectangle->p_min;
            Vector2 botRight = rectangle->p_max;
            Vector3 color = rectangle->color;


            for (int y = 0; y < img.height; y++){
                for (int x = 0; x < img.width; x++) {
                    if (x > topLeft.x && x < botRight.x
                        && y > topLeft.y && y < botRight.y) {
                            img(x,y) = color;
                        }
                }
            }
        }
        else if (auto *triangle = std::get_if<Triangle>(&shape)) {
            Vector2 a = triangle->p0;
            Vector2 b = triangle->p1;
            Vector2 c = triangle->p2;
            Vector3 color = triangle->color;

            for (int y = 0; y < img.height; y++){
                for (int x = 0; x < img.width; x++) {
                    double ABP = (b.x - a.x)*(y - a.y) - (b.y - a.y)*(x - a.x);
                    double BCP = (c.x - b.x)*(y - b.y) - (c.y - b.y)*(x - b.x);
                    double CAP = (a.x - c.x)*(y - c.y) - (a.y - c.y)*(x - c.x);

                    if ((ABP >= 0 && BCP >= 0 && CAP >= 0) || (ABP <= 0 && BCP <= 0 && CAP<=0) ){
                        img(x,y) = color;
                    }
                }
            }
        }
    }

    return img;
}

Image3 hw_1_4(const std::vector<std::string> &params) {
    // Homework 1.4: render transformed shapes
    if (params.size() == 0) {
        return Image3(0, 0);
    }

    Scene scene = parse_scene(params[0]);
    std::cout << scene << std::endl;

    Image3 img(scene.resolution.x, scene.resolution.y);

    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            img(x, y) = scene.background;
        }
    }

    for (Shape shape : scene.shapes) {
        if (auto *circle = std::get_if<Circle>(&shape)){
            Vector3 color = circle->color;
            Real radius = circle->radius;
            Vector2 center = circle->center;

            Matrix3x3 augmentedMatrix = circle->transform;
            Matrix3x3 inverseTransform = inverse(augmentedMatrix);

            for (double y = 0; y < img.height; y++) {
                for (double x = 0; x < img.width; x++) {
                   Vector3 objSpaceP = inverseTransform * Vector3{x,y,1.0};

                    Real distanceX = objSpaceP.x - center.x;
                    Real distanceY = objSpaceP.y - center.y;

                    Real length = std::sqrt(distanceX * distanceX + distanceY * distanceY);

                    if (length <= radius) {
                        img(x,y) = color;
                    }
                }
            }
        }
        else if (auto *rectangle = std::get_if<Rectangle>(&shape)) {
            Vector3 color = rectangle->color;
            Vector2 topLeft = rectangle->p_min;
            Vector2 botRight = rectangle->p_max;

            Matrix3x3 augmentedMatrix = rectangle->transform;
            Matrix3x3 inverseTransform = inverse(augmentedMatrix);

            for (double y = 0; y < img.height; y++){
                for (double x = 0; x < img.width; x++) {
                    Vector3 objSpaceP = inverseTransform * Vector3{x,y,1.0};
                    if (objSpaceP.x > topLeft.x && objSpaceP.x < botRight.x
                        && objSpaceP.y > topLeft.y && objSpaceP.y < botRight.y) {
                            img(x,y) = color;
                        }
                }
            }
        }
        else if (auto *triangle = std::get_if<Triangle>(&shape)) {
            Vector2 a = triangle->p0;
            Vector2 b = triangle->p1;
            Vector2 c = triangle->p2;
            Vector3 color = triangle->color;

            Matrix3x3 augmentedMatrix = triangle->transform;
            Matrix3x3 inverseTransform = inverse(augmentedMatrix);

            for (double y = 0; y < img.height; y++){
                for (double x = 0; x < img.width; x++) {

                    Vector3 objSpaceP = inverseTransform * Vector3{x,y,1.0};

                    double ABP = (b.x - a.x)*(objSpaceP.y - a.y) - (b.y - a.y)*(objSpaceP.x - a.x);
                    double BCP = (c.x - b.x)*(objSpaceP.y - b.y) - (c.y - b.y)*(objSpaceP.x - b.x);
                    double CAP = (a.x - c.x)*(objSpaceP.y - c.y) - (a.y - c.y)*(objSpaceP.x - c.x);

                    if ((ABP >= 0 && BCP >= 0 && CAP >= 0) || (ABP <= 0 && BCP <= 0 && CAP<=0) ){
                        img(x,y) = color;
                    }
                }
            }
        }
    }

    return img;
}

Image3 hw_1_5(const std::vector<std::string> &params) {
    // Homework 1.5: antialiasing
    if (params.size() == 0) {
        return Image3(0, 0);
    }

    Scene scene = parse_scene(params[0]);
    std::cout << scene << std::endl;

    Image3 img(scene.resolution.x, scene.resolution.y);

    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            img(x, y) = scene.background;
        }
    }

    for (Shape shape : scene.shapes) {
        if (auto *circle = std::get_if<Circle>(&shape)){
            Vector3 color = circle->color;
            Real radius = circle->radius;
            Vector2 center = circle->center;

            Matrix3x3 augmentedMatrix = circle->transform;
            Matrix3x3 inverseTransform = inverse(augmentedMatrix);


            //for every pixel (x,y)
                //for every subpixel (i,j)
                    //determine the color of the subpixel
                    //add the color of the subpixel to a total_color
                //get the average color of the pixel 

            for (double y = 0; y < img.height; y++) {
                for (double x = 0; x < img.width; x++) {
                    Vector3 totalColor = {0.0,0.0,0.0};
                    for (double i = 0; i < 4; i++){
                        for (double j = 0; j < 4; j++){
                            Real subX = x + (i + 0.5) / 4.0;
                            Real subY = y + (j + 0.5) / 4.0;

                            Vector3 objSpaceP = inverseTransform * Vector3{subX,subY,1.0};

                            Real distanceX = objSpaceP.x - center.x;
                            Real distanceY = objSpaceP.y - center.y; 

                            Real length = std::sqrt(distanceX * distanceX + distanceY * distanceY);

                            if (length <= radius) {
                                totalColor += color;
                            }
                            else{
                                totalColor += img(subX,subY);
                            }
                        }
                    }
                    Vector3 avgColor = totalColor / 16.0;
                    img(x,y) = avgColor;
                }
            }
        }
        else if (auto *rectangle = std::get_if<Rectangle>(&shape)) {
            Vector3 color = rectangle->color;
            Vector2 topLeft = rectangle->p_min;
            Vector2 botRight = rectangle->p_max;

            Matrix3x3 augmentedMatrix = rectangle->transform;
            Matrix3x3 inverseTransform = inverse(augmentedMatrix);

            for (double y = 0; y < img.height; y++){
                for (double x = 0; x < img.width; x++) {
                    Vector3 totalColor = {0.0,0.0,0.0};
                    for (double i = 0; i < 4.0; i++){
                        for (double j = 0; j < 4.0; j++){
                            Real subX = x + (i + 0.5) / 4.0;
                            Real subY = y + (j + 0.5) / 4.0;

                            Vector3 objSpaceP = inverseTransform * Vector3{subX,subY,1.0};

                            if (objSpaceP.x > topLeft.x && objSpaceP.x < botRight.x
                        && objSpaceP.y > topLeft.y && objSpaceP.y < botRight.y) {
                                totalColor += color;
                            }
                            else{
                                totalColor += img(subX,subY);
                            }

                        }
                    }
                    Vector3 avgColor = totalColor / 16.0;
                    img(x,y) = avgColor;
                    
                }
            }
        }
        else if (auto *triangle = std::get_if<Triangle>(&shape)) {
            Vector2 a = triangle->p0;
            Vector2 b = triangle->p1;
            Vector2 c = triangle->p2;
            Vector3 color = triangle->color;

            Matrix3x3 augmentedMatrix = triangle->transform;
            Matrix3x3 inverseTransform = inverse(augmentedMatrix);

            for (double y = 0; y < img.height; y++){
                for (double x = 0; x < img.width; x++) {
                    Vector3 totalColor = {0.0,0.0,0.0};
                    for (double i = 0; i < 4; i++){
                        for (double j = 0; j < 4; j++){
                            Real subX = x + (i + 0.5) / 4.0;
                            Real subY = y + (j + 0.5) / 4.0;

                            Vector3 objSpaceP = inverseTransform * Vector3{subX,subY,1.0};

                            double ABP = (b.x - a.x)*(objSpaceP.y - a.y) - (b.y - a.y)*(objSpaceP.x - a.x);
                            double BCP = (c.x - b.x)*(objSpaceP.y - b.y) - (c.y - b.y)*(objSpaceP.x - b.x);
                            double CAP = (a.x - c.x)*(objSpaceP.y - c.y) - (a.y - c.y)*(objSpaceP.x - c.x);

                            if ((ABP >= 0 && BCP >= 0 && CAP >= 0) || (ABP <= 0 && BCP <= 0 && CAP<=0) ){
                                totalColor += color;
                            }
                            else{
                                totalColor += img(subX,subY);
                            }
                        }
                    }
                    Vector3 avgColor = totalColor / 16.0;
                    img(x,y) = avgColor;
                }
            }
        }
    }
    return img;
}

Image3 hw_1_6(const std::vector<std::string> &params) {
    // Homework 1.6: alpha blending
    if (params.size() == 0) {
        return Image3(0, 0);
    }

    Scene scene = parse_scene(params[0]);
    std::cout << scene << std::endl;

    Image3 img(scene.resolution.x, scene.resolution.y);

    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            img(x, y) = scene.background;
        }
    }

    Real prevAlpha = 0.0;
    Vector3 prevColor = {0.0,0.0,0.0};
    for (Shape shape : scene.shapes) {
        if (auto *circle = std::get_if<Circle>(&shape)){
            Vector3 color = circle->color;
            Real radius = circle->radius;
            Vector2 center = circle->center;
            Real alpha = circle->alpha;

            Matrix3x3 augmentedMatrix = circle->transform;
            Matrix3x3 inverseTransform = inverse(augmentedMatrix);

            for (double y = 0; y < img.height; y++) {
                for (double x = 0; x < img.width; x++) {
                    Vector3 totalColor = {0.0,0.0,0.0};
                    for (double i = 0; i < 4; i++){
                        for (double j = 0; j < 4; j++){
                            Real subX = x + (i + 0.5) / 4.0;
                            Real subY = y + (j + 0.5) / 4.0;

                            Vector3 objSpaceP = inverseTransform * Vector3{subX,subY,1.0};

                            Real distanceX = objSpaceP.x - center.x;
                            Real distanceY = objSpaceP.y - center.y; 

                            Real length = std::sqrt(distanceX * distanceX + distanceY * distanceY);

                            if (length <= radius) {
                                totalColor += color;
                            }
                            else{
                                totalColor += img(subX,subY);
                            }
                        }
                    }
                    Vector3 avgColor = totalColor / 16.0;
                    Vector3 pixelColor = img(x,y);
                    Vector3 bg = scene.background;
                    if (pixelColor.x == bg.x && pixelColor.y == bg.y && pixelColor.z == bg.z){
                        img(x,y) = alpha*avgColor + (1-alpha)*img(x,y);
                        prevAlpha = alpha;
                        prevColor = avgColor;
                    }
                    else{
                        img(x,y) = alpha*avgColor + prevAlpha*(1-alpha)*prevColor + (1 - alpha) * (1 - prevAlpha) * img(x,y);
                        prevAlpha = alpha;
                        prevColor = avgColor;
                    }
                }
            }
        }
        else if (auto *rectangle = std::get_if<Rectangle>(&shape)) {
            Vector3 color = rectangle->color;
            Vector2 topLeft = rectangle->p_min;
            Vector2 botRight = rectangle->p_max;
            Real alpha = rectangle->alpha;

            Matrix3x3 augmentedMatrix = rectangle->transform;
            Matrix3x3 inverseTransform = inverse(augmentedMatrix);

            for (double y = 0; y < img.height; y++){
                for (double x = 0; x < img.width; x++) {
                    Vector3 totalColor = {0.0,0.0,0.0};
                    for (double i = 0; i < 4.0; i++){
                        for (double j = 0; j < 4.0; j++){
                            Real subX = x + (i + 0.5) / 4.0;
                            Real subY = y + (j + 0.5) / 4.0;

                            Vector3 objSpaceP = inverseTransform * Vector3{subX,subY,1.0};

                            if (objSpaceP.x > topLeft.x && objSpaceP.x < botRight.x
                        && objSpaceP.y > topLeft.y && objSpaceP.y < botRight.y) {
                                totalColor += color;
                            }
                            else{
                                totalColor += img(subX,subY);
                            }

                        }
                    }
                    Vector3 avgColor = totalColor / 16.0;
                    Vector3 pixelColor = img(x,y);
                    Vector3 bg = scene.background;
                    if (pixelColor.x == bg.x && pixelColor.y == bg.y && pixelColor.z == bg.z){
                        img(x,y) = alpha*avgColor + (1-alpha)*img(x,y);
                        prevAlpha = alpha;
                        prevColor = avgColor;
                    }
                    else{
                        img(x,y) = alpha*avgColor + prevAlpha*(1-alpha)*prevColor + (1 - alpha) * (1 - prevAlpha) * img(x,y);
                        prevAlpha = alpha;
                        prevColor = avgColor;
                    }
                }
            }
        }
        else if (auto *triangle = std::get_if<Triangle>(&shape)) {
            Vector2 a = triangle->p0;
            Vector2 b = triangle->p1;
            Vector2 c = triangle->p2;
            Vector3 color = triangle->color;
            Real alpha = triangle->alpha;

            Matrix3x3 augmentedMatrix = triangle->transform;
            Matrix3x3 inverseTransform = inverse(augmentedMatrix);

            for (double y = 0; y < img.height; y++){
                for (double x = 0; x < img.width; x++) {
                    Vector3 totalColor = {0.0,0.0,0.0};
                    for (double i = 0; i < 4; i++){
                        for (double j = 0; j < 4; j++){
                            Real subX = x + (i + 0.5) / 4.0;
                            Real subY = y + (j + 0.5) / 4.0;

                            Vector3 objSpaceP = inverseTransform * Vector3{subX,subY,1.0};

                            double ABP = (b.x - a.x)*(objSpaceP.y - a.y) - (b.y - a.y)*(objSpaceP.x - a.x);
                            double BCP = (c.x - b.x)*(objSpaceP.y - b.y) - (c.y - b.y)*(objSpaceP.x - b.x);
                            double CAP = (a.x - c.x)*(objSpaceP.y - c.y) - (a.y - c.y)*(objSpaceP.x - c.x);

                            if ((ABP >= 0 && BCP >= 0 && CAP >= 0) || (ABP <= 0 && BCP <= 0 && CAP<=0) ){
                                totalColor += color;
                            }
                            else{
                                totalColor += img(subX,subY);
                            }
                        }
                    }
                    Vector3 avgColor = totalColor / 16.0;
                    Vector3 pixelColor = img(x,y);
                    Vector3 bg = scene.background;
                    if (pixelColor.x == bg.x && pixelColor.y == bg.y && pixelColor.z == bg.z){
                        img(x,y) = alpha*avgColor + (1-alpha)*img(x,y);
                        prevAlpha = alpha;
                        prevColor = avgColor;
                    }
                    else{
                        img(x,y) = alpha*avgColor + prevAlpha*(1-alpha)*prevColor + (1 - alpha) * (1 - prevAlpha) * img(x,y);
                        prevAlpha = alpha;
                        prevColor = avgColor;
                    }
                }
            }
        }
    }
    return img;
}
