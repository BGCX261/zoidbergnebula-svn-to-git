#ifndef SHARED_PTR_CONFIG_H
#define SHARED_PTR_CONFIG_H

#ifdef _MSC_VER // other possibilities are WIN32 _WIN32 or _WIN64
#include <memory>
#include <functional>
#else
#include <tr1/memory>
#include <tr1/functional>
#endif

class GameObject;
class SurvivalSite;

typedef std::tr1::shared_ptr<GameObject> GameObject_sp_t;
typedef std::tr1::weak_ptr<GameObject> GameObject_wp_t;
typedef std::tr1::shared_ptr<SurvivalSite> SurvivalSite_sp_t;
typedef std::tr1::weak_ptr<SurvivalSite> SurvivalSite_wp_t;


#endif