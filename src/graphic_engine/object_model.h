#ifndef OBJECT_MODEL_H
#define OBJECT_MODEL_H

#include "voxexporter.h"
#include "voxel.h"
#include <memory>

class ObjectModel
{
public:
    ObjectModel(const std::string& path_to_file);

    const VoxelArray& GetModel() const;

private:
    std::unique_ptr<VoxelArray> model_voxel_array_{nullptr};
};

#endif // OBJECT_MODEL_H
