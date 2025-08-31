import unreal
import os

def export_selected_fbx(base_output_dir):
    # Get selected assets
    assets = unreal.EditorUtilityLibrary.get_selected_assets()

    for asset in assets:
        asset_name = asset.get_name()

        # Make a folder per asset
        asset_folder = os.path.join(base_output_dir, asset_name)
        os.makedirs(asset_folder, exist_ok=True)

        # Build file path
        fbx_path = os.path.join(asset_folder, f"{asset_name}.fbx")

        # Create export task
        task = unreal.AssetExportTask()
        task.object = asset
        task.filename = fbx_path
        task.automated = True
        task.replace_identical = True
        task.prompt = False

        # Use StaticMeshExporterFBX (keeps materials/textures like your reference)
        task.exporter = unreal.StaticMeshExporterFBX()

        # Run task
        unreal.Exporter.run_asset_export_task(task)

        print(f"Exported {asset_name} -> {fbx_path}")

# Run
export_selected_fbx(r"O:/Exports")
