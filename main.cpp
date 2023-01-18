#define _USE_MATH_DEFINES
#include <iostream>
#include <vector>

#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/IO/Options.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>

using MyMesh = OpenMesh::PolyMesh_ArrayKernelT<>;

struct float3 { float x, y, z; };
struct int3 { int i0, i1, i2; };
struct Mesh {
    std::vector<float3> positions;
    std::vector<int3> triangles;
};
struct Box
{
    float3 min;
    float3 max;
    bool inside;
    Box()
    {
        min.x = 0;
        min.y = 0;
        min.z = 0;

        inside = true;

        max.x = 1;
        max.y = 1;
        max.z = 1;
    }

};

struct Input 
{
    Mesh mesh;
};
struct Output 
{
    Mesh mesh;
};

void read(Input& input)
{
    MyMesh mesh;

    auto test = OpenMesh::IO::read_mesh(mesh, "../bunny.obj");
    if (!test)
    {
        std::cerr << "read error\n";
        exit(1);
    }
    else
    {
        std::cout << "Succes\n";
        
        
        if (mesh.is_triangles()) std::cout << "mesh consist of triangles";
        else std::cout << "No triangles!!";

        for (auto vi = mesh.vertices_begin(); vi != mesh.vertices_end(); ++vi)
        {   
            auto point = mesh.point(*vi);

            float3 pnt;
            pnt.x = point[0];
            pnt.y = point[1];
            pnt.z = point[2];

            {
                input.mesh.positions.emplace_back(std::move(pnt));
            }                
        }
    }
}

void write(Output& output) 
{
    // save meshes from output.output_data to separate files
    MyMesh mesh;
    MyMesh::VertexHandle vhandle;
    std::vector<MyMesh::VertexHandle> face_vhandles;

    float min_distance = 100.0;
    auto first = output.mesh.positions.front();
    for (auto vi = output.mesh.positions.begin(); vi != output.mesh.positions.end(); ++vi)
    {
        float3 pnt;
        pnt = (*vi);

        mesh.add_vertex(MyMesh::Point(pnt.x , pnt.y, pnt.z ));

       
        float distance = sqrt ((first.x - pnt.x) * (first.x - pnt.x) +
                               (first.y - pnt.y) * (first.y - pnt.y) + 
                               (first.z - pnt.z) * (first.z - pnt.z)) ;
        if (distance < min_distance)  min_distance = distance;
    }

    // TODO add faces between neighbors
  //  for (auto vi = mesh.vertices_begin(); vi != mesh.vertices_end(); ++vi)


    OpenMesh::IO::Options OptionRead(OpenMesh::IO::Options::VertexTexCoord);

    if (!OptionRead.check(OpenMesh::IO::Options::VertexNormal))
    {
        std::cout << "Normals are absent\n";
        mesh.request_face_normals();
        mesh.update_normals();
        mesh.release_face_normals();
    }
    else
    {
        std::cout << "Normals are present";
    }

    auto test_write = OpenMesh::IO::write_mesh(mesh, "../edited_bunny_3.obj");
    std::cout << "writed new obj\n";
 
}

void intersect(const Input& input, const Box& ConstraintBox, Output& output)
{
    // implement intersection operation.
    MyMesh::VertexHandle vhandle;
    std::vector<MyMesh::VertexHandle> face_vhandles;
    for (auto vi = input.mesh.positions.begin(); vi != input.mesh.positions.end(); ++vi)
    {
        float3 pnt;
        pnt = (*vi);


        if (ConstraintBox.inside == true &&
            (pnt.x > ConstraintBox.min.x && pnt.x < ConstraintBox.max.x) &&
            (pnt.y > ConstraintBox.min.y && pnt.y < ConstraintBox.max.y) &&
            (pnt.z > ConstraintBox.min.z && pnt.z < ConstraintBox.max.z)
            )
        {
            output.mesh.positions.push_back(pnt);
            std::cout << "pnt:" << pnt.x << ' ' << pnt.y << ' ' << pnt.z << "\n";
        }
           
        if (ConstraintBox.inside == false &&
            ((pnt.x < ConstraintBox.min.x || pnt.x > ConstraintBox.max.x) ||
            (pnt.y < ConstraintBox.min.y || pnt.y > ConstraintBox.max.y) ||
            (pnt.z < ConstraintBox.min.z || pnt.z > ConstraintBox.max.z)
            ))
        {
            output.mesh.positions.push_back(pnt);
            std::cout << "pnt:" << pnt.x << ' ' << pnt.y << ' ' << pnt.z << "\n";
        }         
    }
}

void intersect(const Input& input, Output& output)
{
    output.mesh = input.mesh;
}

void main()
{
    Input input;
    read(input);

    Output output;

    Box Constraints;
    Constraints.max.x = 1;
    Constraints.max.y = 1;
    Constraints.max.z = 1;
    Constraints.inside = false;

    intersect(input, Constraints, output);

    write(output);
}
