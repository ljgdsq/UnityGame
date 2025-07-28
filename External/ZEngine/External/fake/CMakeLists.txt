project("fake")

add_library(fake OBJECT fake.cpp
        ../../Framework/Asset/UnknownAsset.h
        ../../Framework/Asset/UnknownAssetLoader.h
        ../../Framework/Editor/Inspector/AssetInspector.h
        ../../Framework/Editor/Inspector/AssetInspector.h
        ../../Framework/Editor/Inspector/MaterialAssetInspector.h
        ../../Framework/Editor/ThumbnailGenerator.h)

target_sources(fake PRIVATE fake.cpp)
