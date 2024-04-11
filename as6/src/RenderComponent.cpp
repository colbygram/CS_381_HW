#include "RenderComponent.h"

#include "Entity.h"
#include "TransformComponent.h"
#include <iostream>

RenderComponent::RenderComponent(Entity* en, raylib::Model* model): Component(en){
    this->model = model;
    selected = false;
}

void RenderComponent::update(float dt){
    //Grab transform component if it exists, else return
    auto ref = object->GetComponent<TransformComponent>();
    if(!ref) return;
    auto& transform = ref->get();
    //Store temporary version of model's transform
    auto temp = model->transform;
    //Convert quaternion to axis angle
    auto [axis, angle] = transform.rotation.ToAxisAngle();
    //Set models new transform
    model->SetTransform(raylib::Transform(model->transform).Translate(transform.position).Rotate(axis,angle));
    model->Draw({});
    if(selected) model->GetTransformedBoundingBox().Draw();
    //Reset old model transform
    model->transform = temp;
}