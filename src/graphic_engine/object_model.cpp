#include "object_model.h"

ObjectModel::ObjectModel(const std::string& path_to_file)
{
    vox::VoxExporter voxModel(path_to_file);

    model_voxel_array_.reset(new VoxelArray(voxModel.getModel(0).GetNumVoxels()));
    unsigned int idx = 0;
    int x_center_shift = voxModel.getModel(0).size_.x / 2;
    int y_center_shift = voxModel.getModel(0).size_.y / 2;

    for(auto voxel : voxModel.getModel(0).voxels_.voxel)
    {
        glm::vec3 position(static_cast<float>(y_center_shift - static_cast<int>(voxel.component.y)),
                           static_cast<float>(x_center_shift - static_cast<int>(voxel.component.x)),
                           static_cast<float>(voxel.component.z)) ;
        vox::Color vColor = voxModel.getColor(voxel.component.colorIdx);
        glm::vec3 color(static_cast<float>(vColor.component.r) / 255.0f,
                        static_cast<float>(vColor.component.g) / 255.0f,
                        static_cast<float>(vColor.component.b) / 255.0f);
        model_voxel_array_->set(idx++, {position, color});
    }
}

const VoxelArray& ObjectModel::GetModel() const {return *model_voxel_array_;}
