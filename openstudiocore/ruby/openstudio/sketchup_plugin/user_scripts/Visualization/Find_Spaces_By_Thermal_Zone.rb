######################################################################
#  Copyright (c) 2008-2015, Alliance for Sustainable Energy.  
#  All rights reserved.
#  
#  This library is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation; either
#  version 2.1 of the License, or (at your option) any later version.
#  
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#  
#  You should have received a copy of the GNU Lesser General Public
#  License along with this library; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
######################################################################

# Each user script is implemented within a class that derives from OpenStudio::Ruleset::UserScript
class FindByThermalZone < OpenStudio::Ruleset::ModelUserScript

  # override name to return the name of your script
  def name
    return "Find Spaces By Thermal Zone"
  end
  
  # returns a vector of arguments, the runner will present these arguments to the user
  # then pass in the results on run
  def arguments(model)
    result = OpenStudio::Ruleset::OSArgumentVector.new
    
    thermal_zone_name = OpenStudio::Ruleset::makeChoiceArgumentOfWorkspaceObjects("thermal_zone_name", "OS_ThermalZone".to_IddObjectType, model, true)
    thermal_zone_name.setDisplayName("Thermal Zone")
    result << thermal_zone_name
    
    return result
  end
    
  # override run to implement the functionality of your script
  # model is an OpenStudio::Model::Model, runner is a OpenStudio::Ruleset::UserScriptRunner
  def run(model, runner, user_arguments)
    super(model, runner, user_arguments)
  
    if not runner.validateUserArguments(arguments(model),user_arguments)
      return false
    end
    
    thermal_zone_handle = runner.getStringArgumentValue("thermal_zone_name",user_arguments)
    # if the user doesn't pick a thermal zone this part of script wont run. Model visibility is left in current state
    
    # get all spaces
    spaces = model.getSpaces

    runner.createProgressBar("Searching Spaces")
    num_total = spaces.size
    num_complete = 0

    # loop over each space
    spaces.each do |space|
      if drawing_interface = space.drawing_interface
        if entity = drawing_interface.entity

          if space.thermalZone.is_initialized 
            thermal_zone = space.thermalZone.get
          else
            next
          end
          
          if thermal_zone_handle == thermal_zone.handle.to_s
            entity.visible = true
          else
            entity.visible = false
          end
          
        end
      end
    end

    runner.destroyProgressBar

    # set hidden to visible so easier for users to select and unide
    Sketchup.active_model.rendering_options["DrawHidden"] = true
    
    return true
  end

end

# this call registers your script with the OpenStudio SketchUp plug-in
FindByThermalZone.new.registerWithApplication
