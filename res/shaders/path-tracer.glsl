#version 460 core

layout (local_size_x = 8, local_size_y = 8) in;
layout (rgba8, binding = 0) uniform image2D screen;

uniform float time;

struct Ray {
    vec3 origin;
    vec3 direction;
};

struct Sphere {
    vec3 center;
    float radius;
    vec3 color;
    bool isLight;
    float reflectivity;
};

Sphere sphere1 = Sphere(vec3(0.0, -0.5, -5.0), 1.0, vec3(1.0, 0.5, 0.2), false, 0.5);
Sphere sphere2 = Sphere(vec3(2.0, 0.5, -3.0), 1.0, vec3(0.0, 1.0, 0.0), false, 0.3);
Sphere sphere3 = Sphere(vec3(-2.0, -1.0, -2.0), 1.0, vec3(0.0, 0.0, 1.0), false, 0.8);
Sphere light = Sphere(vec3(-2, 2, -2), 0.5, vec3(1.8), true, 0.0);

bool intersectSphere(Ray ray, Sphere sphere, out float t) {
    vec3 oc = ray.origin - sphere.center;
    float b = dot(oc, ray.direction);
    float c = dot(oc, oc) - sphere.radius * sphere.radius;
    float h = b * b - c;
    if (h < 0.0) return false;
    h = sqrt(h);
    t = -b - h;
    return t > 0.001;
}

uint seed;
float rand() {
    seed = seed * 1664525u + 1013904223u;
    return float(seed & 0xFFFFFF) / float(0x1000000);
}

vec3 cosineWeightedHemisphere(vec3 normal) {
    float r1 = rand();
    float r2 = rand();
    float phi = 2.0 * 3.14159265 * r1;
    float x = cos(phi) * sqrt(r2);
    float y = sin(phi) * sqrt(r2);
    float z = sqrt(1.0 - r2);

    vec3 tangent = normalize(cross(normal, vec3(0.0, 1.0, 0.0)));
    vec3 bitangent = cross(normal, tangent);
    
    return normalize(x * tangent + y * bitangent + z * normal);
}

vec3 reflectDirection(vec3 normal, vec3 incoming) {
    return normalize(incoming - 2.0 * dot(incoming, normal) * normal);
}

bool inShadow(vec3 point, vec3 lightPos) {
    Ray shadowRay;
    shadowRay.origin = point + normalize(lightPos - point) * 0.05;
    shadowRay.direction = normalize(lightPos - point);

    float t;
    return intersectSphere(shadowRay, sphere1, t) || intersectSphere(shadowRay, sphere2, t) || intersectSphere(shadowRay, sphere3, t);
}

vec3 tracePath(Ray ray) {
    vec3 color = vec3(0.0);
    vec3 throughput = vec3(1.0);
    Ray currentRay = ray;
    vec3 hitPoint;
    vec3 normal;
    Sphere hitObject;
    int maxBounces = 10;

    for (int bounce = 0; bounce < maxBounces; bounce++) {
        float t;
        bool hit = false;

        if (intersectSphere(currentRay, sphere1, t)) {
            hitObject = sphere1;
            hit = true;
        }
        if (intersectSphere(currentRay, sphere2, t)) {
            hitObject = sphere2;
            hit = true;
        }
        if (intersectSphere(currentRay, sphere3, t)) {
            hitObject = sphere3;
            hit = true;
        }
        if (intersectSphere(currentRay, light, t)) {
            hitObject = light;
            hit = true;
        }

        if (!hit) {
            break;
        }

        hitPoint = currentRay.origin + t * currentRay.direction;
        normal = normalize(hitPoint - hitObject.center);

        if (hitObject.isLight) {
            color += throughput * hitObject.color;
            break;
        }

        if (!inShadow(hitPoint, light.center)) {
            vec3 toLight = normalize(light.center - hitPoint);
            float intensity = max(dot(normal, toLight), 0.0);
            color += throughput * hitObject.color * light.color * min(intensity * 0.5, 1.0);
        }

        vec3 reflectedDir = reflectDirection(normal, currentRay.direction);
        Ray reflectedRay = Ray(hitPoint + normal * 0.05, reflectedDir);

        throughput *= hitObject.reflectivity;
        currentRay = reflectedRay;
    }

    return color;
}

void main() {
    float scalar = sin(time);
    light.center += vec3(scalar);
    ivec2 coord = ivec2(gl_GlobalInvocationID.xy);
    ivec2 screenSize = imageSize(screen);

    if (coord.x >= screenSize.x || coord.y >= screenSize.y) return;

    seed = uint(coord.x + coord.y * screenSize.x);

    vec2 uv = (vec2(coord) + vec2(rand(), rand())) / vec2(screenSize) * 2.0 - 1.0;
    uv.x *= float(screenSize.x) / float(screenSize.y);

    Ray ray;
    ray.origin = vec3(0.0, 0.0, 0.0);
    ray.direction = normalize(vec3(uv, -1));

    vec3 finalColor = vec3(0.0);
    int samples = 120;

    for (int i = 0; i < samples; i++) {
        finalColor += tracePath(ray);
    }
    finalColor /= float(samples);

    imageStore(screen, coord, vec4(finalColor, 0));
}