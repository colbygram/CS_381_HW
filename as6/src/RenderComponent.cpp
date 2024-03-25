#include "RenderComponent.h"

#include "Entity.h"
#include "TransformComponent.h"

void RenderComponent::update(float dt){
    auto ref = object->GetComponent<TransformComponent>();
    if(!ref) return;
    auto& transform = ref->get();
    auto [axis, angle] = transform.rotation.ToAxisAngle();

    model.Draw(transform.position, axis, angle);
}